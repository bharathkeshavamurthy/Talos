/*

File    : rfcomm_E-Puck_2.c
Author  : Eliot Ferragni
Date    : 17 november 2017
REV 1.2

Functions to control and use the bluetooth stack
*/

#define __BTSTACK_FILE__ "rfcomm_E-Puck_2.c"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/xtensa_api.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_efuse.h"
#include "btstack.h"
#include "main_e-puck2.h"
#include "rfcomm_e-puck2.h"
#include "button_e-puck2.h"

#define SERVICE_RECORD              0x10001   //service class id (could be everything)
#define SERVICE_BUFFER_SIZE         150
#define HEARTBEAT_PERIOD_MS         1   //call at each loop
#define INITIAL_INCOMMING_CREDITS   1
#define OUTGOING_CREDITS_THRESHOLD  1  


typedef struct {
    uint16_t remote_id;                                 //id assigned by the computer
    uint16_t server_id;                                 //internal id
    uint8_t  spp_service_buffer[SERVICE_BUFFER_SIZE];   //buffer to store the service config
    uint32_t service_record;                            //Config
    int32_t  mtu;
    uint8_t  blue_rx_buffer[BLUE_RX_BUFFER_SIZE];       //rx buffer
    uint8_t  blue_tx_buffer[BLUE_TX_BUFFER_SIZE];       //tx buffer
    ///////////////////////////////////////variables shared between threads/////////////////////////////////////////
    //rx variables
    uint16_t nb_to_read_blue_rx;
    uint16_t remaining_size_blue_rx;
    uint8_t  grant_incomming_credit;
    uint8_t  incomming_credits;
    uint8_t  nb_incomming_credit_to_grant;
    uint8_t* ptr_to_receive_blue_rx;
    uint8_t* ptr_to_read_blue_rx;
    SemaphoreHandle_t xReadBluetooth;
    //tx variables
    uint16_t nb_to_send_blue_tx;
    uint16_t remaining_size_blue_tx;
    uint8_t* ptr_to_send_blue_tx;
    uint8_t* ptr_to_fill_blue_tx;
    SemaphoreHandle_t xWriteBluetooth;
} rfcomm_user_channel_t;             


//internal functions
static uint16_t get_index_from_remote_channel(uint16_t channel_remote_id);
static void init_rf_channels_struct(rfcomm_user_channel_t* channel);
static void init_semaphores(void);
static void spp_service_setup(void);
static void heartbeat_handler(struct btstack_timer_source *ts);
static void one_shot_timer_setup(void);
static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
static void reset_blue_rx(rfcomm_user_channel_t* channel);
static void bluetooth_receive(rfcomm_user_channel_t* channel, uint8_t* buffer, uint16_t buffer_len);
static void reset_blue_tx(rfcomm_user_channel_t* channel);
static void bluetooth_send(rfcomm_user_channel_t* channel);

//names of the channels (virtual serial ports) as seen by the computer
static const char *channels_names[] = {
    "GDB",   //channel 1
    "UART",  //channel 2
    "SPI",   //channel 3
};

////////////////////////////////////////////internal variables/////////////////////////////////////////////////
static btstack_timer_source_t heartbeat;
static btstack_packet_callback_registration_t hci_event_callback_registration;

///////////////////////////////////////variables shared between threads/////////////////////////////////////////
rfcomm_user_channel_t rf_channel[NB_RFCOMM_CHANNELS];
//power mode variables
static uint8_t update_power_mode = 0;
static uint8_t power_mode_bluetooth_state = 0;
static SemaphoreHandle_t xPowerBluetooth = NULL;

//discoverability variables
static uint8_t update_discoverable = 0;
static uint8_t discoverable_bluetooth_state = 0;
static SemaphoreHandle_t xDiscoverableBluetooth = NULL;

//connectivity variables
static uint8_t update_connectable = 0;
static uint8_t connectable_bluetooth_state = 0;
static SemaphoreHandle_t xConnectableBluetooth = NULL;

/* 
 * Return the index of the channel to which correspond the remote channel ID given
 * Necessary because the id given by the remote (computer) is incremented at each connexion and is
 * independant. We need to find each time which channel is linked with the ID given by the computer
 * 
 * returns NB_RFCOMM_CHANNELS if channel not found.
*/
static uint16_t get_index_from_remote_channel(uint16_t channel_remote_id){
    uint16_t ch_used = NB_RFCOMM_CHANNELS;
    uint16_t i = 0;
    for(i = 0 ; i < NB_RFCOMM_CHANNELS ; i++){
        if(xSemaphoreTake(rf_channel[i].xWriteBluetooth, (TickType_t)DELAY_1000_TICKS) == pdTRUE){
            if(rf_channel[i].remote_id == channel_remote_id){
                ch_used = i;
            }
            xSemaphoreGive(rf_channel[i].xWriteBluetooth);
        }
    }
    return ch_used;
}

/* 
 * Init the rfcomm channels' structure
*/
static void init_rf_channels_struct(rfcomm_user_channel_t* channel){
   uint8_t i = 0;

   for(i = 0 ; i < NB_RFCOMM_CHANNELS ; i++){
        channel[i].server_id = (i + 1);                         // must begin at 1
        channel[i].service_record = (SERVICE_RECORD + i + 1);   //increment the service class ID
        //rx variables
        channel[i].nb_to_read_blue_rx = 0;
        channel[i].remaining_size_blue_rx = BLUE_RX_BUFFER_SIZE;
        channel[i].grant_incomming_credit = 0;
        channel[i].incomming_credits = INITIAL_INCOMMING_CREDITS;
        channel[i].nb_incomming_credit_to_grant = 0;
        channel[i].ptr_to_receive_blue_rx = channel[i].blue_rx_buffer;
        channel[i].ptr_to_read_blue_rx = channel[i].blue_rx_buffer;
        channel[i].xReadBluetooth = NULL;
        //tx variables
        channel[i].nb_to_send_blue_tx = 0;
        channel[i].remaining_size_blue_tx = BLUE_TX_BUFFER_SIZE;
        channel[i].ptr_to_send_blue_tx = channel[i].blue_tx_buffer;
        channel[i].ptr_to_fill_blue_tx = channel[i].blue_tx_buffer;
        channel[i].xWriteBluetooth = NULL;
   }
}

/* 
 * Init the semaphores
*/
static void init_semaphores(void){

    for(int i = 0 ; i < NB_RFCOMM_CHANNELS ; i++){
        rf_channel[i].xReadBluetooth = xSemaphoreCreateBinary();
        xSemaphoreGive(rf_channel[i].xReadBluetooth);
        rf_channel[i].xWriteBluetooth = xSemaphoreCreateBinary();
        xSemaphoreGive(rf_channel[i].xWriteBluetooth);
    }

    xPowerBluetooth = xSemaphoreCreateBinary();
    xSemaphoreGive(xPowerBluetooth);

    xDiscoverableBluetooth = xSemaphoreCreateBinary();
    xSemaphoreGive(xDiscoverableBluetooth);

    xConnectableBluetooth = xSemaphoreCreateBinary();
    xSemaphoreGive(xConnectableBluetooth);
}

/* 
 * Set the spp service for the bluetooth
*/
static void spp_service_setup(void){

    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    l2cap_init();

    rfcomm_init();

    // init SDP
    sdp_init();

    for(int i = 0 ; i < NB_RFCOMM_CHANNELS ; i++){
        // reserved channel, mtu limited by l2cap
        rfcomm_register_service_with_initial_credits(packet_handler, rf_channel[i].server_id, 0xffff,INITIAL_INCOMMING_CREDITS);
        //create record for SPP and register with SDP
        memset(rf_channel[i].spp_service_buffer, 0, sizeof(rf_channel[i].spp_service_buffer));
        spp_create_sdp_record(rf_channel[i].spp_service_buffer, rf_channel[i].service_record, rf_channel[i].server_id, channels_names[i]);
        sdp_register_service(rf_channel[i].spp_service_buffer);
        log_info_rfcomm("SDP service n°%d record size: %u\n", (i + 1), de_get_len(rf_channel[i].spp_service_buffer));
    }
}

/* 
 * Handler called peridically by the main of btstack
 * by doing specific stuff here, we make sure it will be sync with
 * the thread of btstack. Unpredictable behaviors appear when functions of
 * btstac are called outside the btstack thread
*/
static void heartbeat_handler(struct btstack_timer_source *ts){

    static uint16_t i = 0;
    for(i = 0; i < NB_RFCOMM_CHANNELS ; i++){
        //check if a send event is required
        if(xSemaphoreTake(rf_channel[i].xWriteBluetooth, (TickType_t)DELAY_10_TICKS) == pdTRUE){
            //we need to test if we are connected, otherwise it floods the stack when not connected
            //and a new reconnexion is nearly impossible to do
            if(rf_channel[i].remote_id){
                if(rf_channel[i].nb_to_send_blue_tx){
                    xSemaphoreGive(rf_channel[i].xWriteBluetooth);
                    rfcomm_request_can_send_now_event(rf_channel[i].remote_id);
                }else{
                    xSemaphoreGive(rf_channel[i].xWriteBluetooth);
                }
            }else{
                xSemaphoreGive(rf_channel[i].xWriteBluetooth);
            }
        }
        //check if incomming credits should be given
        if(xSemaphoreTake(rf_channel[i].xReadBluetooth, (TickType_t)DELAY_10_TICKS) == pdTRUE){
            if(rf_channel[i].grant_incomming_credit){
                rf_channel[i].grant_incomming_credit = NO_UPDATE;
                rf_channel[i].incomming_credits += rf_channel[i].nb_incomming_credit_to_grant;
                rfcomm_grant_credits(rf_channel[i].remote_id, rf_channel[i].nb_incomming_credit_to_grant);
            }
            xSemaphoreGive(rf_channel[i].xReadBluetooth);
        }
    }
    //check if we need to change the state of the power of the bluetooth
    if(xSemaphoreTake(xPowerBluetooth, (TickType_t)DELAY_10_TICKS) == pdTRUE){
        if(update_power_mode){
            update_power_mode = NO_UPDATE;
            hci_power_control(power_mode_bluetooth_state);
        }
        xSemaphoreGive(xPowerBluetooth);
    }
    //check if we need to change the state of the discoverability of the bluetooth
    if(xSemaphoreTake(xDiscoverableBluetooth, (TickType_t)DELAY_10_TICKS) == pdTRUE){
        if(update_discoverable){
            update_discoverable = NO_UPDATE;
            gap_discoverable_control(discoverable_bluetooth_state);
        }
        xSemaphoreGive(xDiscoverableBluetooth);
    }
    //check if we need to change the state of the connectivity of the bluetooth
    if(xSemaphoreTake(xConnectableBluetooth, (TickType_t)DELAY_10_TICKS) == pdTRUE){
        if(update_connectable){
            update_connectable = NO_UPDATE;
            gap_connectable_control(connectable_bluetooth_state);
        }
        xSemaphoreGive(xConnectableBluetooth);
    }
    //reset the one shot timer to call this handler.
    btstack_run_loop_set_timer(ts, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(ts);
} 

/* 
 * Configure the heartbeat handler
*/
static void one_shot_timer_setup(void){
    // set one-shot timer
    heartbeat.process = &heartbeat_handler;
    btstack_run_loop_set_timer(&heartbeat, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(&heartbeat);
}

/* 
 * Function to handle the events related with the bluetooth communication
*/
static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    uint16_t ch_used = 0;

    bd_addr_t event_addr;

    switch (packet_type) {
        case HCI_EVENT_PACKET:
            switch (hci_event_packet_get_type(packet)) {
                case HCI_EVENT_PIN_CODE_REQUEST:
                    // inform about pin code request
                    log_info_rfcomm("Pin code request - using '0000'\n");
                    hci_event_pin_code_request_get_bd_addr(packet, event_addr);
                    gap_pin_code_response(event_addr, "0000");
                    break;

                case HCI_EVENT_USER_CONFIRMATION_REQUEST:
                    // ssp: inform about user confirmation request
                    log_info_rfcomm("SSP User Confirmation Request with numeric value '%06"PRIu32"'\n", little_endian_read_32(packet, 8));
                    log_info_rfcomm("SSP User Confirmation Auto accept\n");
                    break;

                case RFCOMM_EVENT_INCOMING_CONNECTION:
                    // data: event (8), len(8), address(48), channel (8), rfcomm_cid (16)
                    rfcomm_event_incoming_connection_get_bd_addr(packet, event_addr); 
                    ch_used = rfcomm_event_incoming_connection_get_server_channel(packet) - 1;
                    rf_channel[ch_used].remote_id = rfcomm_event_incoming_connection_get_rfcomm_cid(packet);
                    log_info_rfcomm("RFCOMM channel %u requested for %s\n", rf_channel[ch_used].server_id, bd_addr_to_str(event_addr));
                    rfcomm_accept_connection(rf_channel[ch_used].remote_id);
                    break;
               
                case RFCOMM_EVENT_CHANNEL_OPENED:
                    // data: event(8), len(8), status (8), address (48), server channel(8), rfcomm_cid(16), max frame size(16)
                    if (rfcomm_event_channel_opened_get_status(packet)) {
                        log_info_rfcomm("RFCOMM channel open failed, status %u\n", rfcomm_event_channel_opened_get_status(packet));
                    } else {
                        //the channels begin at 1 and the index of the structure at 0. That's why we substract 1
                        ch_used = rfcomm_event_channel_opened_get_server_channel(packet) - 1; 
                        //we collect the ID given by the computer
                        rf_channel[ch_used].remote_id = rfcomm_event_channel_opened_get_rfcomm_cid(packet);
                        //we collect the mtu negociated with the computer
                        rf_channel[ch_used].mtu = rfcomm_event_channel_opened_get_max_frame_size(packet);
                        //we compute how many credits we can grant in one time depending on the rx buffer size
                        rf_channel[ch_used].nb_incomming_credit_to_grant = (BLUE_RX_BUFFER_SIZE / rf_channel[ch_used].mtu);
                        rf_channel[ch_used].incomming_credits = INITIAL_INCOMMING_CREDITS;
                        log_info_rfcomm("RFCOMM channel open succeeded. New RFCOMM Channel ID %u, max frame size %u\n", rf_channel[ch_used].remote_id, rf_channel[ch_used].mtu);
                        //once a connection has been established, we disable the discoverability 
                        //(without effect if it has not bee ativated earlier) and the connectivity
                        //this means nobody else can establish a connection while we already have one
                        bluetooth_discoverable_control(DISABLE);
                        bluetooth_connectable_control(DISABLE);
                    }
                    break;
                case RFCOMM_EVENT_CAN_SEND_NOW:
                    ch_used = get_index_from_remote_channel(channel);
                    if(ch_used < NB_RFCOMM_CHANNELS){
                        bluetooth_send(&rf_channel[ch_used]);
                    }
                    
                    break;
               
                case RFCOMM_EVENT_CHANNEL_CLOSED:
                    //unfortunately, when we close a port on the computer, it disconnects the 
                    //device. So we have to tell each channel it has been disconnected, no matter which channel
                    //generated this event
                    for(int i = 0 ; i < NB_RFCOMM_CHANNELS ; i++){
                        if(xSemaphoreTake(rf_channel[i].xWriteBluetooth, (TickType_t)DELAY_1000_TICKS) == pdTRUE){
                            rf_channel[i].remote_id = 0;
                            log_info_rfcomm("Channel n° %d closed. New remote ID = %d\n",i + 1,rf_channel[i].remote_id);
                            xSemaphoreGive(rf_channel[i].xWriteBluetooth);
                        }
                    }
                    bluetooth_connectable_control(ENABLE);
                    break;
                
                default:
                    break;
            }
            break;

        case RFCOMM_DATA_PACKET:
            log_info_rfcomm("<==================RCV: %d characters =====================>",size);
            log_info_rfcomm("'\n");
            
            ch_used = get_index_from_remote_channel(channel);
            if(ch_used < NB_RFCOMM_CHANNELS){
                bluetooth_receive(&rf_channel[ch_used], packet, size);
            }
            break;

        default:
            break;
    }
}
/* 
 * Reset the pointers and counters of the blue_rx_buffer
 * The buffer must be locked with xSemaphoreTake before calling this function
*/
static void reset_blue_rx(rfcomm_user_channel_t* channel){
    channel->ptr_to_receive_blue_rx = channel->blue_rx_buffer;
    channel->ptr_to_read_blue_rx = channel->blue_rx_buffer;
    channel->remaining_size_blue_rx = BLUE_RX_BUFFER_SIZE;
    channel->nb_to_read_blue_rx = 0;
}

/* 
 * Function triggered by a receive bluetooth event which copy the received datas into the blue_rx_buffer
*/
static void bluetooth_receive(rfcomm_user_channel_t* channel, uint8_t* buffer, uint16_t buffer_len){

    static uint16_t i = 0;

    log_info_rfcomm("incomming_credits = %d\n",channel->incomming_credits);

    if(xSemaphoreTake(channel->xReadBluetooth, (TickType_t)DELAY_1000_TICKS) == pdTRUE){
        log_info_rfcomm("received %d characters \n",buffer_len);
        channel->remaining_size_blue_rx -= buffer_len;
        channel->nb_to_read_blue_rx += buffer_len;
        channel->incomming_credits--;

        for(i = 0 ; i < buffer_len ; i++){
            //the pointer is incremented after the copy of the value
            *channel->ptr_to_receive_blue_rx++ = buffer[i];
        }
        log_info_rfcomm("remaining size = %d\n",channel->remaining_size_blue_rx);
        xSemaphoreGive(channel->xReadBluetooth);
    }
}

int16_t bluetooth_read(CHANNEL_NB channel_nb, uint8_t* buffer, uint16_t buffer_len, int16_t* status){

    rfcomm_user_channel_t* channel = &rf_channel[channel_nb];

    static uint16_t i = 0;
    static uint16_t size_to_read = 0;

    if(!channel->xReadBluetooth){
        *status = BLUETOOTH_NOT_CONNECTED;
        return BLUETOOTH_NOT_CONNECTED;
    }

    if(xSemaphoreTake(channel->xReadBluetooth, (TickType_t)DELAY_1000_TICKS) == pdTRUE){
        if(channel->remote_id){
            if(channel->nb_to_read_blue_rx){

                size_to_read = channel->nb_to_read_blue_rx;
                if(channel->nb_to_read_blue_rx > buffer_len){
                    size_to_read = buffer_len;
                }

                log_info_rfcomm("read %d characters\n",channel->nb_to_read_blue_rx);
                channel->nb_to_read_blue_rx -= size_to_read;
                channel->remaining_size_blue_rx += size_to_read;

                for(i = 0 ; i < size_to_read ; i++){
                    //the pointer is incremented after the copy of the value
                    buffer[i] = *channel->ptr_to_read_blue_rx++;
                }

                log_info_rfcomm("remaining size = %d, nb to read = %d\n",channel->remaining_size_blue_rx, channel->nb_to_read_blue_rx);

                if(!channel->nb_to_read_blue_rx){
                    if(!channel->grant_incomming_credit && !channel->incomming_credits){
                        channel->grant_incomming_credit = UPDATE;
                        log_info_rfcomm("grant credit bluetooth_read\n");
                    }
                    log_info_rfcomm("no more to read => reset ptr\n");
                    reset_blue_rx(channel);
                }
                xSemaphoreGive(channel->xReadBluetooth);
                *status = size_to_read;
                return size_to_read;
            }else{
                xSemaphoreGive(channel->xReadBluetooth);
                log_debug_rfcomm("nothing received\n");
                *status = 0;
                return 0;
            }
        }else{
            xSemaphoreGive(channel->xReadBluetooth);
            log_debug_rfcomm("bluetooth is not connected\n");
            *status = BLUETOOTH_NOT_CONNECTED;
            return BLUETOOTH_NOT_CONNECTED;
        }
    }else{
        log_debug_rfcomm("semaphore not free bluetooth_read\n");
        *status = TASK_COLLISION;
        return TASK_COLLISION;
    }
}

/* 
 * Reset the pointers and counters of the blue_tx_buffer
 * The buffer must be locked with xSemaphoreTake before calling this function
*/
static void reset_blue_tx(rfcomm_user_channel_t* channel){
    channel->ptr_to_send_blue_tx = channel->blue_tx_buffer;
    channel->ptr_to_fill_blue_tx = channel->blue_tx_buffer;
    channel->remaining_size_blue_tx = BLUE_TX_BUFFER_SIZE;
    channel->nb_to_send_blue_tx = 0;
}

/* 
 * Function triggered by a send bluetooth event which send the datas over bluetooth
*/
static void bluetooth_send(rfcomm_user_channel_t* channel){

    static uint16_t credits = 0;
    static uint16_t size_to_send = 0;

    if(xSemaphoreTake(channel->xWriteBluetooth, (TickType_t)DELAY_1000_TICKS) == pdTRUE){
        size_to_send = channel->nb_to_send_blue_tx;
        if(channel->nb_to_send_blue_tx > channel->mtu){
            size_to_send = channel->mtu;
        }

        credits = rfcomm_get_outgoing_credits(channel->remote_id);
        log_info_rfcomm("outgoing credits = %d\n",credits);
        if(credits <= OUTGOING_CREDITS_THRESHOLD){
            //stop the sending during 100ms in order to let the computer digest the datas and 
            //free new credits for us => continue the sending only if we have more than 2 credits
            //send an empty packet to tell to the computer to refresh the credits (needed at least for OS X)
            vTaskDelay(100 / portTICK_PERIOD_MS);
            rfcomm_send(channel->remote_id, channel->ptr_to_send_blue_tx, 0);
        }else{
            rfcomm_send(channel->remote_id, channel->ptr_to_send_blue_tx, size_to_send);
            
            channel->nb_to_send_blue_tx -= size_to_send;

            channel->ptr_to_send_blue_tx += size_to_send;
            log_info_rfcomm(" %d bytes sent, remaining = %d\n",size_to_send, channel->nb_to_send_blue_tx);
        }

        if(channel->nb_to_send_blue_tx > 0){
            log_info_rfcomm("must send another\n");
            xSemaphoreGive(channel->xWriteBluetooth);
            rfcomm_request_can_send_now_event(channel->remote_id);
        }else{
            //reset the pointers
            reset_blue_tx(channel);
            xSemaphoreGive(channel->xWriteBluetooth);
            log_info_rfcomm("must not send => reset ptr\n");
        }
    }
    
}

int16_t bluetooth_write(CHANNEL_NB channel_nb, uint8_t* buffer, uint16_t buffer_len, int16_t* status){

    static uint16_t i = 0;

    rfcomm_user_channel_t* channel = &rf_channel[channel_nb];

    if(!channel->xWriteBluetooth){
        *status = BLUETOOTH_NOT_CONNECTED;
        return BLUETOOTH_NOT_CONNECTED;
    }

    if(xSemaphoreTake(channel->xWriteBluetooth, (TickType_t)DELAY_1000_TICKS) == pdTRUE){
        if(channel->remote_id){
            if(channel->remaining_size_blue_tx >= buffer_len){

                //increment counters for the tx buffer
                channel->nb_to_send_blue_tx += buffer_len;
                channel->remaining_size_blue_tx -= buffer_len;

                //copy datas into the tx buffer
                for(i = 0 ; i < buffer_len ; i++){
                    //the pointer is incremented after the copy of the value
                    *channel->ptr_to_fill_blue_tx++ = buffer[i];
                }
                
                xSemaphoreGive(channel->xWriteBluetooth);

                log_info_rfcomm("wrote %d bytes to send buffer\n", buffer_len);
                log_info_rfcomm("remaining size = %d, nb_to_send = %d\n",channel->remaining_size_blue_tx, channel->nb_to_send_blue_tx);
                *status = DATAS_WRITTEN;
                return DATAS_WRITTEN;
            }else{
                xSemaphoreGive(channel->xWriteBluetooth);
                log_debug_rfcomm("not enough space\n");
                *status = BUFFER_FULL;
                return BUFFER_FULL;
            }
        }else{
            xSemaphoreGive(channel->xWriteBluetooth);
            log_debug_rfcomm("bluetooth is not connected\n");
            *status = BLUETOOTH_NOT_CONNECTED;
            return BLUETOOTH_NOT_CONNECTED;
        }
        
    }else{
        log_debug_rfcomm("semaphore not free bluetooth_write\n");
        *status = TASK_COLLISION;
        return TASK_COLLISION;
    }
}

void bluetooth_power_control(HCI_POWER_MODE power_mode){
    if(xSemaphoreTake(xPowerBluetooth, (TickType_t)DELAY_1000_TICKS) == pdTRUE){
        power_mode_bluetooth_state = power_mode;
        update_power_mode = UPDATE;
        xSemaphoreGive(xPowerBluetooth);
    }
}

void bluetooth_discoverable_control(CONTROL_STATE state){
    if(xSemaphoreTake(xDiscoverableBluetooth, (TickType_t)DELAY_1000_TICKS) == pdTRUE){
        discoverable_bluetooth_state = state;
        update_discoverable = UPDATE;
        xSemaphoreGive(xDiscoverableBluetooth);
    }
}

void bluetooth_connectable_control(CONTROL_STATE state){
    if(xSemaphoreTake(xConnectableBluetooth, (TickType_t)DELAY_1000_TICKS) == pdTRUE){
        connectable_bluetooth_state = state;
        update_connectable = UPDATE;
        xSemaphoreGive(xConnectableBluetooth);
    }
}

void example_echo_bluetooth_task_channel_1(void *pvParameter){
  uint8_t test_buf[2000];
  uint16_t size = 2000;
  int16_t status;

  for(int i = 0 ; i < size ; i++){
    test_buf[i] = i;
  }
  while(1){
    vTaskDelay(10 / portTICK_PERIOD_MS);
    int16_t rcv = bluetooth_read(CHANNEL_1, test_buf, size, &status);
    //for(int j = 0 ; j < 96 ; j++){
    if(rcv>0){
        while(bluetooth_write(CHANNEL_1, test_buf, rcv, &status) != DATAS_WRITTEN){
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
    //vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
void example_echo_bluetooth_task_channel_2(void *pvParameter){
  uint8_t test_buf[2000];
  uint16_t size = 2000;
  int16_t status;

  for(int i = 0 ; i < size ; i++){
    test_buf[i] = i;
  }
  while(1){
    vTaskDelay(10 / portTICK_PERIOD_MS);
    int16_t rcv = bluetooth_read(CHANNEL_2, test_buf, size, &status);
    //for(int k = 0 ; k < 96 ; k++){
    if(rcv>0){
        while(bluetooth_write(CHANNEL_2, test_buf, rcv, &status) != DATAS_WRITTEN){
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
    //vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
void example_echo_bluetooth_task_channel_3(void *pvParameter){
  uint8_t test_buf[2000];
  uint16_t size = 2000;
  int16_t status;

  for(int i = 0 ; i < size ; i++){
    test_buf[i] = i;
  }
  while(1){
    vTaskDelay(10 / portTICK_PERIOD_MS);
    int16_t rcv = bluetooth_read(CHANNEL_3, test_buf, size, &status);
    //for(int k = 0 ; k < 96 ; k++){
    if(rcv>0){
        while(bluetooth_write(CHANNEL_3, test_buf, rcv, &status) != DATAS_WRITTEN){
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
    //vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

/* 
 * Function to configure and initiate the bluetooth communication
 * called by btstack_main located in /components/btstack/main.c
*/
int btstack_setup(int argc, const char * argv[]){
	static char bt_name[14];
	uint32_t blk3_rdata0, blk3_rdata1;
	uint64_t temp;
	uint16_t robot_id;
	
    //init the channels used
    init_rf_channels_struct(rf_channel);
    //init the semaphores used
    init_semaphores();
    //init the timer used by the heartbeat handler
    one_shot_timer_setup();
    //init the differents software stacks used to provide virtual serial ports over bluetooth
    spp_service_setup();
    //set the authentification process
    gap_ssp_set_io_capability(SSP_IO_CAPABILITY_DISPLAY_ONLY);

	 // The id of the robot is stored in the first 64 bits of the block3 of efuses.
	 // Each id is 2 bytes long, thus there is space for writing 4 times the id in case there is the need to change it.
	 // The old id need to be programmed to 0xFFFF in order to be identified as invalid.
	blk3_rdata0 = REG_READ(EFUSE_BLK3_RDATA0_REG);
	blk3_rdata1 = REG_READ(EFUSE_BLK3_RDATA1_REG);
	temp = ((uint64_t)blk3_rdata1<<32)|blk3_rdata0;
	for(int i=0; i<4; i++) {
		robot_id = (temp>>(i*16))&0xFFFF;
		if(robot_id != 0xFFFF) {
			break;
		}
	}	
	//set the name of the device as seen by the computer the zeros are replaced by the mac adress
	sprintf(bt_name, "e-puck2_%05d", robot_id);
	gap_set_local_name(bt_name);

    //enable the discoverability of the bluetooth if the button is pressed during the startup
    if(button_is_pressed()){
        gap_discoverable_control(ENABLE);
    }

    // turn on the state machine
    hci_power_control(HCI_POWER_ON);

    return 0;
}


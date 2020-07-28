#include <iostream>
#include "epuck2.h"
#include <stdio.h>
#include <math.h>
#include <chrono>

Epuck2::Epuck2() {
    memset(port_name, 0x00, 50);
    memset(input_buffer, 0x00, INPUT_BUFF_SIZE);
    memset(output_buffer, 0x00, OUTPUT_BUFF_SIZE);
    output_buffer[0] = 0xF7; //-0x09;
    output_buffer[20] = 0xF8; //-0x08;
    output_buffer[21] = 0x00;
}

void Epuck2::clearCommunication(void) {
    char data[100];
    comm->flush();

    sprintf(data, "\r");
    comm->writeData(data, 1, 10000);    // clear output buffer
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
    comm->readData(data, 100, 200000);

    comm->flush();

    sprintf(data, "V\r");
    comm->writeData(data, 2, 10000);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    comm->readData(data, 55, 200000);

    //std::cout << "clear comm: " << data << std::endl;
}

int8_t Epuck2::establishConnection(char* portName) {
	int8_t err = 0;

    memcpy(port_name, portName, strlen(portName));

    comm = new SerialComm();
    err = comm->connect(portName);
    if(err < 0) {
        return err;
    }
    clearCommunication();
	return 0;
}

// It would be better to add a parameter to let the user choose what frequency this thread should run at maximum.
// This is to free a bit of bandwidth in the Bluetooth channel.
void Epuck2::commThread(void) {
    int32_t mantis = 0;
    int16_t exp = 0;
    int16_t num_bytes = 0;
    while(1) {
        comm->flush();
        mutexTx.lock();
        num_bytes = comm->writeData((char*)output_buffer, OUTPUT_BUFF_SIZE, 1000000);
        mutexTx.unlock();
        //std::cout << "bytes written = " << num_bytes << std::endl;
        //comm->flush();
        mutexRx.lock();
        num_bytes = comm->readData((char*)input_buffer, INPUT_BUFF_SIZE, 100000);
        //std::cout << "bytes read = " << num_bytes << std::endl;
        if(num_bytes == 0) {
            mutexRx.unlock();
            closeConnection();
            establishConnection(port_name);
            continue;
        } else if(num_bytes < INPUT_BUFF_SIZE) {
            mutexRx.unlock();
            continue;
        }

		last_msg_sent_flag = 1;	// Set the flag here because we are sure that this is the response to the last message sent, thus we are sure the message was actually sent and received by the robot.

        acc_raw[0] = (input_buffer[1]<<8) + input_buffer[0];
        acc_raw[1] = (input_buffer[3]<<8) + input_buffer[2];
        acc_raw[2] = (input_buffer[5]<<8) + input_buffer[4];

        mantis = (input_buffer[6] & 0xff) + ((input_buffer[7] & 0xffl) << 8) + (((input_buffer[8] &0x7fl) | 0x80) << 16);
        exp = (input_buffer[9] & 0x7f) * 2 + ((input_buffer[8] & 0x80) ? 1 : 0);
        if (input_buffer[9] & 0x80) {
            mantis = -mantis;
        }
        acceleration = (mantis || exp) ? ((float) ldexp (mantis, (exp - 127 - 23))): 0;

        mantis = (input_buffer[10] & 0xff) + ((input_buffer[11] & 0xffl) << 8) + (((input_buffer[12] &0x7fl) | 0x80) << 16);
        exp = (input_buffer[13] & 0x7f) * 2 + ((input_buffer[12] & 0x80) ? 1 : 0);
        if (input_buffer[13] & 0x80) {
            mantis = -mantis;
        }
        orientation = (mantis || exp) ? ((float) ldexp (mantis, (exp - 127 - 23))): 0;
        if (orientation < 0.0 ) {
            orientation = 0.0;
        }
        if (orientation > 360.0 ) {
            orientation = 360.0;
        }

        mantis = (input_buffer[14] & 0xff) + ((input_buffer[15] & 0xffl) << 8) + (((input_buffer[16] &0x7fl) | 0x80) << 16);
        exp = (input_buffer[17] & 0x7f) * 2 + ((input_buffer[16] & 0x80) ? 1 : 0);
        if (input_buffer[17] & 0x80) {
            mantis = -mantis;
        }
        inclination = (mantis || exp) ? ((float) ldexp (mantis, (exp - 127 - 23))): 0;
        if (inclination < 0.0 ) {
            inclination = 0.0;
        }
        if (inclination > 180.0 ) {
            inclination = 180.0;
        }

        gyro_raw[0] = (input_buffer[19]<<8) + input_buffer[18];
        gyro_raw[1] = (input_buffer[21]<<8) + input_buffer[20];
        gyro_raw[2] = (input_buffer[23]<<8) + input_buffer[22];

		/*
        magnetic_field[0] = *((float*)&input_buffer[24]);
        magnetic_field[1] = *((float*)&input_buffer[28]);
        magnetic_field[2] = *((float*)&input_buffer[32]);
		*/
		
		heading = input_buffer[24];
		imu_mag_value_x = input_buffer[25];
		
        temperature = input_buffer[36];

        proximity[0] =  (input_buffer[38]<<8) + input_buffer[37];
        proximity[1] =  (input_buffer[40]<<8) + input_buffer[39];
        proximity[2] =  (input_buffer[42]<<8) + input_buffer[41];
        proximity[3] =  (input_buffer[44]<<8) + input_buffer[43];
        proximity[4] =  (input_buffer[46]<<8) + input_buffer[45];
        proximity[5] =  (input_buffer[48]<<8) + input_buffer[47];
        proximity[6] =  (input_buffer[50]<<8) + input_buffer[49];
        proximity[7] =  (input_buffer[52]<<8) + input_buffer[51];

        ambient[0] =  (input_buffer[54]<<8) + input_buffer[53];
        ambient[1] =  (input_buffer[56]<<8) + input_buffer[55];
        ambient[2] =  (input_buffer[58]<<8) + input_buffer[57];
        ambient[3] =  (input_buffer[60]<<8) + input_buffer[59];
        ambient[4] =  (input_buffer[62]<<8) + input_buffer[61];
        ambient[5] =  (input_buffer[64]<<8) + input_buffer[63];
        ambient[6] =  (input_buffer[66]<<8) + input_buffer[65];
        ambient[7] =  (input_buffer[68]<<8) + input_buffer[67];

        distance_mm = (uint16_t)(((uint8_t)input_buffer[70]<<8)|((uint8_t)input_buffer[69]));

        mic_vol[0] =  (input_buffer[72]<<8) + input_buffer[71];
        mic_vol[1] =  (input_buffer[74]<<8) + input_buffer[73];
        mic_vol[2] =  (input_buffer[76]<<8) + input_buffer[75];
        mic_vol[3] =  (input_buffer[78]<<8) + input_buffer[77];

        mot_steps[LEFT] =  (input_buffer[80]<<8) + input_buffer[79];
        mot_steps[RIGHT] =  (input_buffer[82]<<8) + input_buffer[81];

        batt_raw = (input_buffer[84]<<8) + input_buffer[83];

        micro_sd_state = input_buffer[85];

        tv_remote_toggle = input_buffer[86];
        tv_remote_addr = input_buffer[87];
        tv_remote_data = input_buffer[88];

        selector = input_buffer[89];

        ground_proximity[0] = (input_buffer[91]<<8) + input_buffer[90];
        ground_proximity[1] = (input_buffer[93]<<8) + input_buffer[92];
        ground_proximity[2] = (input_buffer[95]<<8) + input_buffer[94];
        ground_ambient[0] =  (input_buffer[97]<<8) + input_buffer[96];
        ground_ambient[1] =  (input_buffer[99]<<8) + input_buffer[98];
        ground_ambient[2] =  (input_buffer[101]<<8) + input_buffer[100];

        mutexRx.unlock();
    }
}

int16_t Epuck2::getHeading(void) {
	return heading;
}

int16_t Epuck2::getImuMagValueX(void) {
	return imu_mag_value_x;
}

int8_t Epuck2::startCommunication(char* portName) {
    int8_t err = 0;
    err = establishConnection(portName);
    if(err < 0) {
        return err;
    }
	comm_thread = std::thread (&Epuck2::commThread, this);
    return err;
}

void Epuck2::closeConnection(void) {
    if(comm!=NULL) {
        comm->disconnect();
        comm=NULL;
    }
}

void Epuck2::stopCommunication() {
    closeConnection();
	if(comm_thread.joinable()) {
		comm_thread.~thread();
	}
}

char* Epuck2::getPortName(void) {
	return port_name;
}

int16_t Epuck2::getAccelerometerRaw(uint8_t id) {
    if(id > 2) {
        return 0;
    } else {
        return acc_raw[id];
    }
}

float Epuck2::getAcceleration(void) {
    return acceleration;
}

float Epuck2::getOrientation(void) {
    return orientation;
}

float Epuck2::getInclination(void) {
    return inclination;
}

int16_t Epuck2::getGyroRaw(uint8_t id) {
    if(id > 2) {
        return 0;
    } else {
        return gyro_raw[id];
    }
}

/*
float Epuck2::getMagneticField(uint8_t id) {
    if(id > 2) {
        return 0;
    } else {
        return magnetic_field[id];
    }
}
*/

int8_t Epuck2::getTemperature(void) {
    return temperature;
}

uint16_t Epuck2::getProximity(uint8_t id) {
    if(id > 7) {
        return 0;
    } else {
        return proximity[id];
    }
}

uint16_t Epuck2::getAmbient(uint8_t id) {
    if(id > 7) {
        return 0;
    } else {
        return ambient[id];
    }
}

uint16_t Epuck2::getDistanceMillimeters(void) {
    return distance_mm;
}

uint16_t Epuck2::getMicVolume(uint8_t id) {
    if(id > 3) {
        return 0;
    } else {
        return mic_vol[id];
    }
}

int16_t Epuck2::getMotorSteps(uint8_t id) {
    return mot_steps[id];
}

uint16_t Epuck2::getBatteryRaw(void) {
    return batt_raw;
}

uint8_t Epuck2::getSdState(void) {
    return micro_sd_state;
}

uint8_t Epuck2::getTvRemoteData(void) {
    return tv_remote_data;
}

uint8_t Epuck2::getSelector(void) {
    return selector;
}

uint16_t Epuck2::getGroundProximity(uint8_t id) {
    if(id > 2) {
        return 0;
    } else {
        return ground_proximity[id];
    }
}

uint16_t Epuck2::getGroundAmbient(uint8_t id) {
    if(id > 2) {
        return 0;
    } else {
        return ground_ambient[id];
    }
}

void Epuck2::setSpeed(int16_t left, int16_t right) {
    mutexTx.lock();
    output_buffer[2] = left & 0xFF;
    output_buffer[3] = (left>>8) & 0xFF;
    output_buffer[4] = right & 0xFF;
    output_buffer[5] = (right>>8) & 0xFF;
    mutexTx.unlock();
}

void Epuck2::setLed(uint8_t id, uint8_t state) {
    mutexTx.lock();
    if(state) {
        output_buffer[6] |= (1<<id);
    } else {
        output_buffer[6] &= ~(1<<id);
    }
    mutexTx.unlock();
}

void Epuck2::setRgbLeds(uint8_t red2, uint8_t green2, uint8_t blue2, uint8_t red4, uint8_t green4, uint8_t blue4, uint8_t red6, uint8_t green6, uint8_t blue6, uint8_t red8, uint8_t green8, uint8_t blue8) {
    mutexTx.lock();
    output_buffer[7] = red2;
    output_buffer[8] = green2;
    output_buffer[9] = blue2;
    output_buffer[10] = red4;
    output_buffer[11] = green4;
    output_buffer[12] = blue4;
    output_buffer[13] = red6;
    output_buffer[14] = green6;
    output_buffer[15] = blue6;
    output_buffer[16] = red8;
    output_buffer[17] = green8;
    output_buffer[18] = blue8;
    mutexTx.unlock();
}

void Epuck2::setSound(uint8_t id) {
    mutexTx.lock();
    output_buffer[19] = id;
    mutexTx.unlock();
}

uint8_t Epuck2::waitForUpdate(uint32_t us) {
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

	mutexRx.lock();
    last_msg_sent_flag = 0;
	mutexRx.unlock();

    while(last_msg_sent_flag == 0) {
		now = std::chrono::system_clock::now();
		if(std::chrono::duration_cast<std::chrono::microseconds>(now - start).count() > us) {
			return 1;
		}
    }

    return 0;
}

Epuck2::~Epuck2() {
    stopCommunication();
}

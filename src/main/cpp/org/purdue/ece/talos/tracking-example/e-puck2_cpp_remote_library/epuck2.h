#ifndef EPUCK2_H
#define EPUCK2_H

#include "SerialComm.h"
#include <thread>
#include <mutex>

#define INPUT_BUFF_SIZE 103
#define OUTPUT_BUFF_SIZE 22

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define MIC_RIGHT 0
#define MIC_LEFT 1
#define MIC_BACK 2
#define MIC_FRONT 3

#define LEFT 0
#define RIGHT 1

#define GROUND_LEFT 0
#define GROUND_CENTER 1
#define GROUND_RIGHT 2

#define LED1 0
#define LED3 1
#define LED5 2
#define LED7 3
#define BODY_LED 4
#define FRONT_LED 5

#define SOUND_MUTE 0x00
#define SOUND_MARIO 0x01
#define SOUND_UNDERWORLD 0x02
#define SOUND_STARWARS 0x04
#define SOUND_4KHZ 0x08
#define SOUND_10KHZ 0x10
#define SOUND_STOP 0x20

/**
 * The Epuck2 class represents an e-puck2 robot.
 *
 * \author Stefano Morgani
 * \version 1.0
 * \date 07/11/19
 */
class Epuck2
{
    public:
        Epuck2();
        virtual ~Epuck2();

        /**
        * \brief To be called once at the beginning, it handles the communication with the robot (receive sensors data and send actuators values).
        * \param portName name of the serial port, must be null terminated.
        * \return 0 if communication started correctly, < 0 in case of errors.
        */
		int8_t startCommunication(char* portName);

		/**
        * \brief To be called once at the end, it closes the Bluetooth communication and stop the communication thread.
        * \return none
        */
		void stopCommunication(void);

        /**
        * \brief This function can be used to know what serial port the robot is connected.
        * \return serial port name.
        */		
		char * getPortName(void);
		
		uint8_t waitForUpdate(uint32_t us);

		/**
        * \brief Accelerometer raw axes values.
        * \param id X_AXIS, Y_AXIS or Z_AXIS.
        * \return values between -1500 and 1500, resolution is +/- 2g.
        */
		int16_t getAccelerometerRaw(uint8_t id);

		/**
        * \brief Accelerometer magnitude.
        * \return values between 0.0 and about 2600.0 (about 3.45 g).
        */
		float getAcceleration(void);

		/**
        * \brief Orientation of the robot.
        * \return values between 0.0 and 360.0 (degrees).
        */
		float getOrientation(void);

		/**
        * \brief Inclination of the robot.
        * \return values between 0.0 and 90.0 degrees (when tilted in any direction)
        */
		float getInclination(void);

		/**
        * \brief Gyroscope raw axes values.
        * \param id X_AXIS, Y_AXIS or Z_AXIS.
        * \return values between -32768 and 32767, range is +/- 250 dps.
        */
		int16_t getGyroRaw(uint8_t id);

		/**
        * \brief Magnetometer raw axes values.
        * \param id X_AXIS, Y_AXIS or Z_AXIS.
        * \return values in float, range is +/- 4912.0 uT (magnetic flux density expressed in micro Tesla).
        */
		float getMagneticField(uint8_t id);

		/**
        * \brief Temperature given from the IMU.
        * \return Celsius degrees.
        */
		int8_t getTemperature(void);

		/**
        * \brief IR proximity sensors values.
        * \param id id of the proximity from which to get the value, refer to https://www.gctronic.com/doc/index.php?title=e-puck2#Overview for the position of the proximity sensors.
        * \return values between 0 (no objects detected) and 4095 (object near the sensor).
        */
        uint16_t getProximity(uint8_t id);

        /**
        * \brief IR proximity sensors values (ambient light measure).
        * \param id id of the proximity from which to get the value, refer to https://www.gctronic.com/doc/index.php?title=e-puck2#Overview for the position of the proximity sensors.
        * \return values between 0 (strong light) and 4095 (dark).
        */
        uint16_t getAmbient(uint8_t id);

        /**
        * \brief Time of flight sensor value.
        * \return distance measured in millimeters.
        */
        uint16_t getDistanceMillimeters(void);

        /**
        * \brief Microphones volume.
        * \param id MIC_RIGHT, MIC_LEFT, MIC_BACK, MIC_FRONT.
        * \return values between 0 and 4095.
        */
        uint16_t getMicVolume(uint8_t id);

        /**
        * \brief Motors steps: 1000 steps per wheel revolution.
        * \param id LEFT, RIGHT.
        * \return motor steps.
        */
        int16_t getMotorSteps(uint8_t id);

        /**
        * \brief Battery sampled value from the ADC.
        * \return sampled value.
        */
        uint16_t getBatteryRaw(void);

        /**
        * \brief Micro SD state.
        * \return 1 if the micro SD is present and can be read/write, 0 otherwise.
        */
        uint8_t getSdState(void);

        /**
        * \brief TV remote received data (RC5 protocol).
        * \return received command.
        */
        uint8_t getTvRemoteData(void);

        /**
        * \brief Selector position (16 positions available).
        * \return values between 0 and 15.
        */
        uint8_t getSelector(void);

		/**
        * \brief IR proximity sensors values from the ground sensors extension.
        * \param id id of the proximity from which to get the value: GROUND_LEFT, GROUND_CENTER, GROUND_RIGHT.
        * \return values between 0 (no surface at all or not reflective surface e.g. black) and 1023 (very reflective surface e.g. white).
        */
        uint16_t getGroundProximity(uint8_t id);

        /**
        * \brief IR proximity sensors values (ambient light measure) from the ground sensors extension.
        * \param id id of the proximity from which to get the value: GROUND_LEFT, GROUND_CENTER, GROUND_RIGHT.
        * \return values between 0 (strong light) and 1023 (dark).
        */
        uint16_t getGroundAmbient(uint8_t id);

        /**
        * \brief Set motors speed.
        * \param left desired left speed (-1000..1000)
        * \param right desired right speed (-1000..1000)
        * \return none.
        */
		void setSpeed(int16_t left, int16_t right);

        /**
        * \brief Turn on/off the LEDs around the robots, the body LED and the front LED. Refer to https://www.gctronic.com/doc/index.php?title=e-puck2#Overview for the position of the LEDs.
        * \param id LED1, LED3, LED5, LED7, BODY_LED, FRONT_LED.
        * \param state 0=off, 1=on
        * \return none.
        */
		void setLed(uint8_t id, uint8_t state);

        /**
        * \brief Set the color and power of the RGB LEDs around the robots. Refer to https://www.gctronic.com/doc/index.php?title=e-puck2#Overview for the position of the RGB LEDs.
        * \param red2 RGB LED2 red component (0..100).
        * \param green2 RGB LED2 green component (0..100).
        * \param blue2 RGB LED2 blue component (0..100).
        * \param red4 RGB LED4 red component (0..100).
        * \param green4 RGB LED4 green component (0..100).
        * \param blue4 RGB LED4 blue component (0..100).
        * \param red6 RGB LED6 red component (0..100).
        * \param green6 RGB LED6 green component (0..100).
        * \param blue6 RGB LED6 blue component (0..100).
        * \param red8 RGB LED8 red component (0..100).
        * \param green8 RGB LED8 green component (0..100).
        * \param blue8 RGB LED8 blue component (0..100).
        * \return none.
        */
		void setRgbLeds(uint8_t red2, uint8_t green2, uint8_t blue2, uint8_t red4, uint8_t green4, uint8_t blue4, uint8_t red6, uint8_t green6, uint8_t blue6, uint8_t red8, uint8_t green8, uint8_t blue8);

        /**
        * \brief Start playing predefined sound tracks or specific frequencies. The play can also be stopped.
        * \param id SOUND_MUTE (no sound), SOUND_MARIO, SOUND_UNDERWORLD, SOUND_STARWARS, SOUND_4KHZ, SOUND_10KHZ, SOUND_STOP (stop the currently played sound if any).
        * \return none.
        */
		void setSound(uint8_t id);

    protected:

    private:
        /**
        * \brief To be called once at the beginning, it opens the Bluetooth (serial) connection with the robot.
        * \param portName name of the serial port.
        * \return 0 if connection established correctly, < 0 in case of errors.
        */
		int8_t establishConnection(char* portName);

        /**
        * \brief It closes the Bluetooth (serial) connection with the robot.
        * \return none.
        */
		void closeConnection(void);

        /**
        * \brief To be called once a connection with the robot is established. It sends some initial commands to the robot and read the related responses in order to flush the input/output buffers.
        * \return none.
        */
		void clearCommunication(void);

        /**
        * \brief Function called in a separate thread. Inside this function the data with the robot are exchanged and the received data are parsed.
        * \return none.
        */		
		void commThread(void);

        char port_name[50]; /**< Bluetooth connection port name (last update) */
		uint8_t output_buffer[OUTPUT_BUFF_SIZE]; /**< Transmission buffer: set all actuators (0x09) + actuators data (19 bytes, refer to https://www.gctronic.com/doc/index.php?title=e-puck2_PC_side_development#WiFi_2) + get all sensors (0x08) + end delimiter (0x00) */
		uint8_t input_buffer[INPUT_BUFF_SIZE]; /**< Receive buffer: sensors data (103 bytes (104 - 1 (button handled by ESP32)), refer to https://www.gctronic.com/doc/index.php?title=e-puck2_PC_side_development#WiFi_2) */
		int16_t acc_raw[3]; /**< Accelerometer raw values (last update) */
		float acceleration; /**< Acceleration of the robot (last update) */
		float orientation; /**< Orientation of the robot (last update) */
		float inclination; /**< Inclination of the robot (last update) */
        int16_t gyro_raw[3]; /**< Gyroscope raw values (last update) */
        float magnetic_field[3]; /**< Magnetic field raw values (last update) */
        int8_t temperature; /**< Temperature (last update) */
		uint16_t proximity[8]; /**< IR proximity values (last update) */
		uint16_t ambient[8]; /**< IR proximity ambient values (last update) */
        uint16_t distance_mm; /**< Time of flight distance (last update) */
        uint16_t mic_vol[4]; /**< Microphones volumes (last update) */
        int16_t mot_steps[2]; /**< Motors steps (last update) */
        uint16_t batt_raw; /**< Battery raw value (last update) */
        uint8_t micro_sd_state; /**< Micro SD state (last update) */
        uint8_t tv_remote_toggle, tv_remote_addr, tv_remote_data; /**< TV remote RC5 protocol values (last update) */
        uint8_t selector; /**< Selector position (last update) */
        uint16_t ground_proximity[3]; /**< Ground extension IR proximity values (last update) */
		uint16_t ground_ambient[3]; /**< Ground extension IR proximity ambient values (last update) */
		uint8_t last_msg_sent_flag; /**< Flag indicating whether the last message was sent and received by the robot. */
		SerialComm *comm;	/**< Pointer to the serial port for the Bluetooth device*/
		std::mutex mutexTx; /**< Mutex on the transmission buffer. */
		std::mutex mutexRx; /**< Mutex on the receive buffer. */
		std::thread comm_thread; /**< Communication thread handle. */


};

#endif // EPUCK2_H

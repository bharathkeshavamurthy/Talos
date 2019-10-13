import serial
# Establish a serial connection with the robot over Bluetooth
INTERFACE_ID = '/dev/tty.e-puck2_04490-UART'
print('[INFO] Bluetooth: Establishing a serial communication link with the robot over Bluetooth. '
      'Interface ID: {}'.format(INTERFACE_ID))
serial_connection = serial.Serial(INTERFACE_ID,
                                  115200,
                                  timeout=0)
# The reply from the sensors initialization...
reply = []
# The transient motor reply member
transient_reply = None
# Forever...
while True:
    try:
        # Start moving the robot forward...
        print('[INFO] Motor Actuator Control: Moving the robot forward')
        serial_connection.write(bytes(b'D,1000,1000\r\n'))
        transient_reply = serial_connection.read()
        # Get proximity sensor data
        serial_connection.write(bytes(b'N\r\n'))
        reply.append(serial_connection.read(20))
        print('[INFO] Proximity Sensor Data: {}'.format(str([str(k) for k in reply])))
        # Get the IR receiver data
        serial_connection.write(bytes(b'G\r\n'))
        reply = []
        while len(reply) < 3:
            reply.append(serial_connection.read())
        print('[INFO] IR Receiver Data: {}'.format(str([str(k) for k in reply])))
    except serial.serialutil.SerialException as e:
        print('[ERROR] SerialException: The device has been disconnected!')
        break

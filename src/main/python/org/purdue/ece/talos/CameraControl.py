import serial
# Establish a serial connection with the robot over USB 2.0
INTERFACE_ID = '/dev/cu.usbmodem3011'
print('[INFO] USB: Establishing a serial communication link with the robot over USB. '
      'Interface ID: {}'.format(INTERFACE_ID))
serial_connection = serial.Serial(INTERFACE_ID,
                                  115200,
                                  timeout=0)
# The camera parameters
reply = []
try:
    # Motor Control
    serial_connection.flush()
    serial_connection.write(bytes(b'D,1000,1000\r\n'))
    # Set the camera parameters
    serial_connection.flush()
    serial_connection.write(bytes(b'J,1,10,10,1,10,10\r\n'))
    while True:
        # Get the camera parameters
        serial_connection.flush()
        serial_connection.write(bytes(b'I\r\n'))
        reply.append(serial_connection.read())
        while len(reply) < 106:
            reply.append(serial_connection.read())
        print('[INFO] Camera: Received Camera Data = {}'.format(str(reply)))
except serial.serialutil.SerialException as e:
    print('[ERROR] SerialException: The device has been disconnected!')

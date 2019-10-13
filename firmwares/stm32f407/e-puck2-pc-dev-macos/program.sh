#/bin/bash

#python -c "import serial.tools.list_ports"

#if [ $? -eq 0 ]; then
#    echo "pyserial installed"
#else
#    echo "pyserial not installed"
#    sudo easy_install pip
#    pip install pyserial
#fi

port=$(python3 list_com_ports.py);
if [ "$port" = "" ]; then
    echo "Robot not detected, be sure the robot is connected to the computer"
    exit
fi
echo "Robot detected on port $port";

filename=$(find . -name *.elf)
if [ "$filename" = "" ]; then
    echo "Firmware not found, be sure you put the firmware (.elf) inside this directory"
    exit
fi
echo "Robot will be updated with firmare $filename";

gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gdb --interpreter=mi -nx \
-ex "target extended-remote $port" \
-ex "monitor swdp_scan" \
-ex "attach 1" \
-ex "set mem inaccessible-by-default off" \
-ex "load" \
$filename \
-ex "set confirm off" \
-ex quit





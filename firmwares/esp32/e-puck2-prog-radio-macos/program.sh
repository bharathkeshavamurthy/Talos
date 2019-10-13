#/bin/bash

port=$(python3 list_com_ports.py);
if [ "$port" = "" ]; then
    echo "Robot not detected, be sure the robot is connected to the computer"
    exit
fi
echo "Robot detected on port $port";

if [ -f "bootloader.bin" ]; then
    bootloader="bootloader.bin"
else
    echo "Cannot find bootloader.bin"
    exit
fi

if [ -f "partitions_singleapp.bin" ]; then
    partitions="partitions_singleapp.bin"
else
    echo "Cannot find partitions_singleapp.bin"
    exit
fi

if [ -f "ESP32_E-Puck_2.bin" ]; then
    main="ESP32_E-Puck_2.bin"
else
    echo "Cannot find ESP32_E-Puck_2.bin"
    exit
fi

python3 esptool.py --chip esp32 --port $port --baud 230400 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 $bootloader 0x10000 $main 0x8000 $partitions

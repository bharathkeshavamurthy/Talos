@echo off
setlocal

if exist "bootloader.bin" (
    set bootloader="bootloader.bin"
) else (
    echo "Cannot find bootloader.bin"
)

if exist "partitions_singleapp.bin" (
    set partitions="partitions_singleapp.bin"
) else (
    echo "Cannot find partitions_singleapp.bin"
)

if exist "ESP32_E-Puck_2.bin" (
    set main="ESP32_E-Puck_2.bin"
) else (
    echo "Cannot find ESP32_E-Puck_2.bin"
)

echo %bootloader%
echo %partitions%
echo %main%

for /f %%i in ('list_com_ports.exe') do set "port=%%i"
echo %port%

esptool.exe --chip esp32 --port %port% --baud 230400 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 %bootloader% 0x10000 %main% 0x8000 %partitions%

::pause
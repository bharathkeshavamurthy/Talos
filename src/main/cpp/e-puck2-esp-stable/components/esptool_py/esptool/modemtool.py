#!/usr/bin/env python

from __future__ import print_function, division
from inspect import currentframe, getframeinfo

import argparse
import hashlib
import inspect
import os
import serial
import struct
import sys
import time
import base64
import zlib
import shlex
import copy
import io

__version__ = "1.0"

DEFAULT_TIMEOUT = 3       # timeout for most flash operations
SYNC_TIMEOUT = 0.1        # timeout for syncing with bootloader
DEFAULT_PORT = "/dev/ttyUSB0"
DEFAULT_PORT = "COM8"

ser = serial.Serial()

def mydebug():
    frameinfo = getframeinfo(currentframe())
    print(frameinfo.filename, frameinfo.lineno)

class ESPLoader(object):
    """ Base class providing access to ESP ROM & softtware stub bootloaders.
    Subclasses provide ESP8266 & ESP32 specific functionality.

    Don't instantiate this base class directly, either instantiate a subclass or
    call ESPLoader.detect_chip() which will interrogate the chip and return the
    appropriate subclass instance.

    """
    CHIP_NAME = "Espressif device"
    IS_STUB = False
    ESP_ROM_BAUD    = 115200
    def __init__(self, port=DEFAULT_PORT, baud=ESP_ROM_BAUD):
        """Base constructor for ESPLoader bootloader interaction

        Don't call this constructor, either instantiate ESP8266ROM
        or ESP32ROM, or use ESPLoader.detect_chip().

        This base class has all of the instance methods for bootloader
        functionality supported across various chips & stub
        loaders. Subclasses replace the functions they don't support
        with ones which throw NotImplementedInROMError().

        """
        if isinstance(port, serial.Serial):
            self._port = port
        else:
            self._port = serial.serial_for_url(port)
        self._set_port_baudrate(baud)

    def _set_port_baudrate(self, baud):
        try:
            self._port.baudrate = baud
        except IOError:
            raise FatalError("Failed to set baud rate %d. The driver may not support this rate." % baud)

    def _set_modem(self, dtr, rts):
        """ A single connection attempt, with esp32r0 workaround options """
        self._port.setDTR(dtr)  # IO0=HIGH
        self._port.setRTS(rts)   # EN=LOW, chip in reset

        self._port.timeout = SYNC_TIMEOUT
        self._port.flushInput()
        self._port.flushOutput()
        self._port.timeout = DEFAULT_TIMEOUT

    def hard_reset(self):
        self._port.setRTS(True)  # EN->LOW
        time.sleep(0.1)
        self._port.setRTS(False)

# set up a dictionary of actions

def clearDTR ():
    ser.setDTR(False)
    #ser.flushInput()
    #ser.flushOutput()

def setDTR ():
    ser.setDTR(True)
    # ser.flushInput()
    # ser.flushOutput()

def clearRTS ():
    ser.setRTS(False)
    # ser.flushInput()
    # ser.flushOutput()

def setRTS ():
    ser.setRTS(True)
    # ser.flushInput()
    # ser.flushOutput()

def DTR_RTS_0 ():
    ser.setRTS(False)
    ser.setDTR(False)
    # ser.flushInput()
    # ser.flushOutput()

def DTR_RTS_1 ():
    ser.setRTS(True)
    ser.setDTR(False)
    # ser.flushInput()
    # ser.flushOutput()

def DTR_RTS_2 ():
    ser.setRTS(False)
    ser.setDTR(True)
    # ser.flushInput()
    # ser.flushOutput()

def DTR_RTS_3 ():
    ser.setRTS(True)
    ser.setDTR(True)
    # ser.flushInput()
    # ser.flushOutput()

def nothing (): print("")
def errhandler ():
   print("Your input has not been recognised")

takeaction = {
    'd': clearDTR,
    'D': setDTR,
    'r': clearRTS,
    'R': setRTS,
    '0': DTR_RTS_0,
    '1': DTR_RTS_1,
    '2': DTR_RTS_2,
    '3': DTR_RTS_3,
    'q': exit,
    'Q': exit,
    '\n': nothing
    }



def main():
    parser = argparse.ArgumentParser(description='modemtool.py v%s - DTR/RTS Utility' % __version__, prog='modemtool')

    parser.add_argument('--port', '-p',
                        help='Serial port device',
                        default=DEFAULT_PORT)

    args = parser.parse_args()

    print('modemtool.py v%s' % __version__)
    #frameinfo = getframeinfo(currentframe())
    #print(frameinfo.filename, frameinfo.lineno)
    print('Serial port is ',args.port)
#    ser = serial.Serial()
    ser.port = args.port
    ser.open()
    """
    if ser.isOpen()
        print(ser.name," is open")
    else
        print(ser.name," is not open !!")
    """
    print("Keys map")
    print("d|D : clear|set DTR then send to virtual port")
    print("r|R : clear|set RTS then send to virtual port")
    print("0 : clear DTR and RTS then send to virtual port")
    print("1 : clear DTR and set RTS then send to virtual port")
    print("2 : set DTR and clear RTS then send to virtual port")
    print("3 : set DTR and RTS then send to virtual port")
    print("q|Q : Quit the program")
    sys.stdout.flush()
    while 1:
        sys.stdout.flush()
        """
        - Position the Cursor:
          \033[<L>;<C>H
             Or
          \033[<L>;<C>f
          puts the cursor at line L and column C.
        - Move the cursor up N lines:
          \033[<N>A
        - Move the cursor down N lines:
          \033[<N>B
        - Move the cursor forward N columns:
          \033[<N>C
        - Move the cursor backward N columns:
          \033[<N>D
        - Clear the screen, move to (0,0):
          \033[2J
        - Erase to end of line:
          \033[K
        - Save cursor position:
          \033[s
        - Restore cursor position:
          \033[u
        """
        # ch = raw_input()
        ch = input()
        print("\033[3A")
        print("\033[1D")
        takeaction.get(ch,errhandler)()

def _main():
    main()
    sys.exit(2)


if __name__ == '__main__':
    _main()

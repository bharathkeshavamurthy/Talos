/*
 *  SerialComm.cpp
 *  EPuckMonitor
 *
 *  Created by Stefano Morgani on 11/18/08.
 *
 *	Copyright 2008 GCtronic
 *
 *  This file is part of EPuckMonitor.
 *
 *  EPuckMonitor is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  EPuckMonitor is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EPuckMonitor; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "SerialComm.h"

SerialComm::SerialComm() {

}

SerialComm::~SerialComm() {

}

int SerialComm::connect(char *path) {
#if defined(_WIN32) || defined(_WIN64)
    std::string m_CommPort(path);
    DCB m_dcb;
    COMMTIMEOUTS  m_TimeOuts;
    m_CommPort = "\\\\.\\" + m_CommPort;
    m_hCom = CreateFileA(m_CommPort.c_str(),
                        GENERIC_READ | GENERIC_WRITE,
                        0,    /* comm devices must be opened w/exclusive-access */
                        NULL, /* no security attrs */
                        OPEN_EXISTING, /* comm devices must use OPEN_EXISTING */
                        0,    /* not overlapped I/O */
                        NULL  /* hTemplate must be NULL for comm devices */
                        );

    // If CreateFile fails, throw an exception. CreateFile will fail if the
    // port is already open, or if the com port does not exist.
    if(m_hCom == INVALID_HANDLE_VALUE) {
        return -1;
    }

    // Now get the DCB properties of the port we just opened
    if(!GetCommState(m_hCom,&m_dcb))
    {
        // something is hay wire, close the port and return
        CloseHandle(m_hCom);
        return -1;
    }

    // dcb contains the actual port properties.  Now copy our settings into this dcb
    m_dcb.BaudRate  =  115200;
    m_dcb.ByteSize  =  8;
    m_dcb.Parity    =  NOPARITY;
    m_dcb.StopBits  =  ONESTOPBIT;
    m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
    m_dcb.fDtrControl = DTR_CONTROL_DISABLE;
    m_dcb.fBinary   = true;
    m_dcb.fParity   = NOPARITY;
    m_dcb.fAbortOnError = false;

    // now we can set the properties of the port with our settings.
    if(!SetCommState(m_hCom,&m_dcb))
    {
        // something is hay wire, close the port and return
        CloseHandle(m_hCom);
        return -1;
    }

    // set the intial size of the transmit and receive queues. These are
    // not exposed to outside clients of the class either. Perhaps they should be?
    // I set the receive buffer to 32k, and the transmit buffer to 9k (a default).
    if(!SetupComm(m_hCom, 1024*32, 1024*9))
    {
        // something is hay wire, close the port and return
        CloseHandle(m_hCom);
        return -1;
    }

    // These values are just default values that I determined empirically.
    // Adjust as necessary. I don't expose these to the outside because
    // most people aren't sure how they work (uhhh, like me included).
    m_TimeOuts.ReadIntervalTimeout         = MAXDWORD; //15;
    m_TimeOuts.ReadTotalTimeoutMultiplier  = 0;
    m_TimeOuts.ReadTotalTimeoutConstant    = 0; //250;
    m_TimeOuts.WriteTotalTimeoutMultiplier = 1;
    m_TimeOuts.WriteTotalTimeoutConstant   = 250;

    if(!SetCommTimeouts(m_hCom, &m_TimeOuts))
    {
        // something is hay wire, close the port and return
        CloseHandle(m_hCom);
        return -1;
    }
    return 0;
#else
    int err = 0;
    speed_t baud_rate = B115200;

    // open the USB stream
    fd = open(path, O_RDWR | O_NONBLOCK | O_NDELAY);
    if(fd==-1){
        return -1;
    }
    // get the baud rate and the flags
    struct termios term;
    tcgetattr(fd, &term);

    err = cfsetispeed(&term, baud_rate);
    if(err != 0) {
        std::cerr << "error setting input speed" << std::endl;
    }
    err = cfsetospeed(&term, baud_rate);
    if(err != 0) {
        std::cerr << "error setting output speed" << std::endl;
    }

    term.c_iflag = 0;
    term.c_oflag = 0;
    term.c_cflag = CLOCAL | CREAD | CS8;
    term.c_lflag = 0;
    term.c_cc[ VTIME ] = 0; //timeout in tenths of a second
    term.c_cc[ VMIN ] = 0;  //no wait for any char

    if( tcsetattr(fd, TCSAFLUSH, &term) < 0)	//termios flags setting
    {
        perror("tcsetattr: ");
    }

    return 0;
#endif
}

void SerialComm::disconnect() {
#if defined(_WIN32) || defined(_WIN64)
    FlushFileBuffers(m_hCom);
    PurgeComm(m_hCom,PURGE_RXCLEAR);
    CloseHandle(m_hCom);
#else
	tcflush(fd,TCIOFLUSH);
	close(fd);
#endif
}

void SerialComm::flush() {
#if defined(_WIN32) || defined(_WIN64)
    FlushFileBuffers(m_hCom);
    PurgeComm(m_hCom,PURGE_RXCLEAR);
#else
	tcflush(fd,TCIOFLUSH);
#endif
}

int32_t SerialComm::writeData(char *buf, int num_bytes, int usleep_time) {
#if defined(_WIN32) || defined(_WIN64)
    DWORD dummy;
    if((num_bytes == 0) || (buf == NULL)) {
        return 0;
    }
    if(!WriteFile(m_hCom, buf, num_bytes, &dummy, NULL)) {
        return -1;
    }
    return dummy;
#else
	uint32_t bytes_written = 0;
	tcflush(fd,TCIOFLUSH);				//flush the input and output buffers before writing new data/commands
	int current_bytes_written = 0;
	errno = 0;
	int timeout = usleep_time/70;		//wait 70 useconds between every read; the loop lasts for usleep_time/70 times
	while(bytes_written < num_bytes) {
		current_bytes_written = write(fd, &buf[bytes_written], num_bytes-bytes_written);
		if(current_bytes_written >= 0) {	//if write ok
			bytes_written += current_bytes_written;
		} //else {
			//FILE *fp = fopen("write_error.txt", "a");
			//fprintf(fp, "Error writing to device: %s\n", strerror(errno));
			//fclose(fp);
			//errno=0;
		//}
		if(bytes_written < num_bytes) {
			std::this_thread::sleep_for(std::chrono::microseconds(70));				//1/(115200/8) = 70 useconds per byte
			timeout--;
		}
		if(timeout==0) {
			break;
		}
	}
	return bytes_written;
#endif
}

int32_t SerialComm::readData(char *buf, int num_bytes, uint32_t usleep_time) {
	int32_t bytes_read=0;
#if defined(_WIN32) || defined(_WIN64)
    DWORD currentBytesRead = 0;
    uint32_t timeout = usleep_time/1000;		//wait 100 useconds between every read (Bluetooth need 70usec/bytes); the loop lasts for usleep_time/70 times
    while(bytes_read < num_bytes) {
        if(!ReadFile(m_hCom, &buf[bytes_read], num_bytes-bytes_read, &currentBytesRead, NULL)) {
            return -1;
        }
        bytes_read += currentBytesRead;
        if(bytes_read < num_bytes) {	//wait to receive something new
            Sleep(1);				    //1 ms
            timeout--;
        }
        if(timeout <= 0) {
            break;
        }
    }
#else

	int current_bytes_read=0;
	errno=0;
	int timeout = usleep_time/70;		//wait 70 useconds between every read; the loop lasts for usleep_time/70 times
	while(bytes_read<num_bytes) {
		current_bytes_read=read(fd,&buf[bytes_read],num_bytes-bytes_read);
		if(current_bytes_read>=0) {	//if read ok
			bytes_read+=current_bytes_read;
		} //else {
			//FILE *fp = fopen("read_error.txt", "a");
			//fprintf(fp, "Error reading from device: %s (code %d)\n", strerror(errno), errno);
			//fclose(fp);
			//errno=0;
		//}
		if(bytes_read<num_bytes) {	//wait to receive something new
			std::this_thread::sleep_for(std::chrono::microseconds(70));				//1/(115200/8) = 70 useconds per byte
			timeout--;
		}
		if(timeout==0) {
			break;
		}
	}
#endif
	return bytes_read;
}

void SerialComm::discard(int num_bytes) {
	char *temp = (char *)malloc(num_bytes*sizeof(char));
#if defined(_WIN32) || defined(_WIN64)
    readData(temp, num_bytes, 100000);
#else
	int bytes_red=0;
	int current_bytes_read=0;
	errno=0;
	while(bytes_red<num_bytes) {
		current_bytes_read=read(fd,&temp[bytes_red],num_bytes-bytes_red);
		if(current_bytes_read>=0) {	//if read ok
			bytes_red+=current_bytes_read;
		} //else {
			//FILE *fp = fopen("discard_error.txt", "a");
			//fprintf(fp, "Error discarding data: %s\n", strerror(errno));
			//fclose(fp);
			//errno=0;
		//}
	}
#endif
	free(temp);
	return;
}

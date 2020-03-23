#if defined(_WIN32) || defined(_WIN64)
    #include "windows.h"
    #include <winsock2.h>
#endif

#ifndef NETWORK_CONNECTION_H_
#define NETWORK_CONNECTION_H_

#ifdef __cplusplus
extern "C" {
#endif

//CONNECTTOHOST – Connects to a remote host
signed char OpenTCPConnection(int PortNo, char* IPAddress);

void CloseTCPConnection();

signed int ReceiveTCP(char *server_reply);


#ifdef __cplusplus
}
#endif

#endif // NETWORK_CONNECTION_H_




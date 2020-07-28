#if defined(_WIN32) || defined(_WIN64)
    #include "windows.h"
#endif

#ifndef UTILITY_H_
#define UTILITY_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
void curPos(int x, int y);
void clearTerminal();
#endif

void updateTime(SYSTEMTIME *time, FILETIME *timeF, ULONGLONG *time64);

ULONGLONG diffTimeSeconds(ULONGLONG timeStart, ULONGLONG timeEnd);

ULONGLONG diffTimeMilliseconds(ULONGLONG timeStart, ULONGLONG timeEnd);

ULONGLONG diffTimeMicroSeconds(ULONGLONG timeStart, ULONGLONG timeEnd);

unsigned char SpaceKeyPressed();

unsigned char QKeyPressed();


#ifdef __cplusplus
}
#endif

#endif // NETWORK_CONNECTION_H_




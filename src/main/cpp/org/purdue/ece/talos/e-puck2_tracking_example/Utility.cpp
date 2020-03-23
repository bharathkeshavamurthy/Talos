
#include "Utility.h"
#if defined(_WIN32) || defined(_WIN64)
    #include "windows.h"
#endif
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
void curPos(int x, int y) {
  HANDLE hStdout;
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
  csbiInfo.dwCursorPosition.X=x;
  csbiInfo.dwCursorPosition.Y=y;
  SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition);
}

void clearTerminal() {
    system( "cls" );
}
#endif

void updateTime(SYSTEMTIME *time, FILETIME *timeF, ULONGLONG *time64) {
    GetSystemTime(time);
    SystemTimeToFileTime(time, timeF);
    *time64 = (((ULONGLONG) (*timeF).dwHighDateTime) << 32) + (*timeF).dwLowDateTime;
}

ULONGLONG diffTimeSeconds(ULONGLONG timeStart, ULONGLONG timeEnd) {
    return ((timeEnd-timeStart)/10000000);
}

ULONGLONG diffTimeMilliseconds(ULONGLONG timeStart, ULONGLONG timeEnd) {
    return ((timeEnd-timeStart)/10000);
}

ULONGLONG diffTimeMicroSeconds(ULONGLONG timeStart, ULONGLONG timeEnd) {
    return ((timeEnd-timeStart)/10);
}

unsigned char QKeyPressed() {

#if defined(_WIN32) || defined(_WIN64)
    if(GetKeyState (0x51) < 0) {     // 'q'
        return 1;
    } else {
        return 0;
    }
#endif

}

unsigned char SpaceKeyPressed() {

#if defined(_WIN32) || defined(_WIN64)
    if(GetKeyState (VK_SPACE) < 0) {     // spacebar
        return 1;
    } else {
        return 0;
    }
#endif

}

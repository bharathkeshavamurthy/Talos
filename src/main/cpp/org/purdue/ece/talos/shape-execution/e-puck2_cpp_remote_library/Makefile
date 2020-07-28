CC = "C:/Program Files (x86)/CodeBlocks/MinGW/bin/g++.exe"
AR = "C:/Program Files (x86)/CodeBlocks/MinGW/bin/ar.exe"
RM = del

all:
	$(CC) -c SerialComm.cpp epuck2.cpp -std=c++11
	$(AR) -r libepuck2bt.a SerialComm.o epuck2.o

clean:
	$(RM) *.o
	$(RM) *.a

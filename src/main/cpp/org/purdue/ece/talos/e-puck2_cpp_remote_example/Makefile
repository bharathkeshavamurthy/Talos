CC = "C:/Program Files (x86)/CodeBlocks/MinGW/bin/mingw32-g++.exe"
RM = rm
SRC = main.cpp
TARGET = main
INCLUDES = -I. -I../e-puck2_cpp_remote_library
LIBS = -L. -L../e-puck2_cpp_remote_library
CFLAGS = $(INCLUDES) $(LIBS) -std=c++11
LFLAGS = -lepuck2bt -lpthread
DEPS =
OBJS =

%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS)

all: $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LFLAGS)

clean:
	$(RM) -f $(TARGET)

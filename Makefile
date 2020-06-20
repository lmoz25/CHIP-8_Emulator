CC=g++

CCFLAGS+=-I./include/
CCFLAGS+=-Wall -Werror -g
CCFLAGS+=-std=c++17
LDFLAGS+=-L/usr/lib/x86_64-linux-gnu/ -lglut -lGLEW -lGL -lSDL2

EMULATOR=chip8.exe

all: $(EMULATOR)

$(EMULATOR): Chip8.o main.o Key.o Graphics.o
	$(CC) $^ $(CCFLAGS) $(LDFLAGS) -o $@

%.o: src/%.cpp
	$(CC) $^ $(CCFLAGS) -c -o $@

clean: 
	rm *.o $(EMULATOR)

.PHONY: all clean
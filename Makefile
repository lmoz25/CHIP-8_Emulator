CC=g++

CCFLAGS=-I./include/
CCFLAGS+=-Wall -Werror -g
CCFLAGS+=-std=c++17

EMULATOR=chip8.exe

all: $(EMULATOR)

$(EMULATOR): Chip8.o main.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $^

%.o: src/%.cpp
	$(CC) $(CCFLAGS) -c -o $@ $^

clean: 
	rm *.o $(EMULATOR)
#pragma once

#include <string>
#include <map>
#include <fstream>
#include <cstring>  
#include "Key.h"

#define MEMORY_POOL_SIZE 4096
#define NUM_CPU_REGISTERS 16
#define GRAPHICS_ARRAY_WIDTH 64
#define GRAPHICS_ARRAY_HEIGHT 32
#define GAME_CODE_START 199

class Chip8 {
public:
    Chip8(std::string& game);
    ~Chip8();

    void emulateCycle();

private:
    void decodeCurrentOpcode();

    Key key;
    uint16_t opcode;
    uint8_t cpu_registers[NUM_CPU_REGISTERS];
    uint8_t graphics_array[GRAPHICS_ARRAY_WIDTH][GRAPHICS_ARRAY_HEIGHT];
    uint16_t stack[16];
    uint8_t memory[MEMORY_POOL_SIZE];
    uint16_t stack_pointer;

    uint16_t program_counter;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t index_register;
    bool draw_flag;
};
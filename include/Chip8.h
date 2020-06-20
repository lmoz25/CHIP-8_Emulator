#pragma once

#include <string>
#include <map>
#include <fstream>
#include <cstring>
#include <memory>

#include "Key.h"
#include "Graphics.h"
#include "Logger.h"

#define MEMORY_POOL_SIZE 4096
#define NUM_CPU_REGISTERS 16
#define GAME_CODE_START 199
#define FONTSET_SIZE 80

class Chip8 {
public:
    Chip8();
    ~Chip8();
    void loadGame(const std::string& path);
    uint8_t getDebugMem(){
        return memory[0x200];
    }

    void emulateCycle();
    void reshapeWindow(GLsizei w, GLsizei h);
    void keyboardUp(unsigned char key, int x, int y);
    void keyboardDown(unsigned char key, int x, int y);

private:
    void processCurrentOpcode();
    void updateTimers();
    uint16_t getCharLocation(uint8_t character);

    std::array<uint8_t, MEMORY_POOL_SIZE> memory;
    uint16_t program_counter;
    uint16_t opcode;
    uint16_t stack_pointer;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t index_register;
    Key key;
    std::array<uint8_t, NUM_CPU_REGISTERS> cpu_registers;
    // C array makes using SDL easier
    uint8_t graphics_array[GRAPHICS_ARRAY_WIDTH*GRAPHICS_ARRAY_HEIGHT];
    std::array<uint16_t, 16> stack;
    bool draw_flag;
    std::unique_ptr<Graphics> graphics;
};



// ---*----
// ------*-
// -*-----*

// 3,0 , 6,1 , 1,2 , 7,2

// ---*----------*--*-----*

// 3, 14, 
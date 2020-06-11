#include "Opcode.h"
#include <string>
#include <map>

#define MEMORY_POOL_SIZE 4096
#define NUM_CPU_REGISTERS 16
#define GRAPHICS_ARRAY_WIDTH 64
#define GRAPHICS_ARRAY_HEIGHT 32

class Chip8 {
public:
    Chip8(std::string& game);
    ~Chip8();

    void emulateCycle();

private:
    void decodeCurrentOpcode();

    uint8_t opcode;
    uint8_t cpu_registers[NUM_CPU_REGISTERS];
    uint8_t graphics_array[GRAPHICS_ARRAY_HEIGHT][GRAPHICS_ARRAY_WIDTH];
    uint16_t stack[16];
    uint8_t memory[MEMORY_POOL_SIZE];
    uint16_t stack_pointer;

    uint8_t program_counter;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t index_register;
};
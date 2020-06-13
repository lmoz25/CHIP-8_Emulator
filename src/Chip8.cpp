#include "Chip8.h"
#include <iostream>

Chip8::Chip8(std::string& game) 
    : program_counter(0x200), opcode(0), stack_pointer(0), delay_timer(0), sound_timer(0), index_register(0){
    
    memset(&memory, 0, sizeof(uint8_t)*MEMORY_POOL_SIZE);
    memset(&graphics_array, 0, GRAPHICS_ARRAY_HEIGHT*GRAPHICS_ARRAY_WIDTH*sizeof(uint8_t));
    memset(&stack, 0, sizeof(uint16_t)*16);

    std::ifstream gamefile;
    gamefile.open(game.c_str());
    uint8_t ch = gamefile.get();

    uint16_t i = program_counter;

    while (gamefile.good() && i < MEMORY_POOL_SIZE) {
        memory[i++] = ch;
        ch = gamefile.get();
        // fprintf(stdout, "0x%x\n", ch);
    }
    gamefile.close();
}

Chip8::~Chip8(){}

void Chip8::emulateCycle() {

    decodeCurrentOpcode();
    
}

void Chip8::decodeCurrentOpcode() {
    opcode = memory[program_counter] << 8 | memory[program_counter+1];
    bool skip_next_instruction = false;
    switch(opcode & 0xF000) {
        case 0x1000:
            program_counter = opcode & 0x0FFF;
            index_register = NUM_CPU_REGISTERS;
            return;
        case 0x2000:
            // Call routine at 0x2000 somehow
            break;
        case 0x3000: {
            if((opcode & 0x00FF) == cpu_registers[(opcode & 0x0F00) >> 8]) {
                skip_next_instruction = true;
            }
            break;
        }
        case 0x4000: {
            if((opcode & 0x00FF) != cpu_registers[(opcode & 0x0F00) >> 8]) {
                skip_next_instruction = true;
            }
            break;
        }

        case 0x5000: {
            if(cpu_registers[(opcode & 0x0F00) >> 8] == cpu_registers[(opcode & 0x00F0) >> 4]) {
                skip_next_instruction = true;
            }
            break;
        }

        case 0x6000:
            cpu_registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            break;
        
        case 0x7000:
            cpu_registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;

        case 0x8000:{
            uint8_t register1 = (opcode & 0x0F00) >> 8;
            uint8_t register2 = (opcode & 0x00F0) >> 4;
            switch (opcode & 0x000F)
            {
            case 0x00:
                cpu_registers[register1] = cpu_registers[register2];
                break;

            case 0x01:
                cpu_registers[register1] |= cpu_registers[register2];
                break;

            case 0x02:
                cpu_registers[register1] &= cpu_registers[register2];
                break;

            case 0x03:
                cpu_registers[register1] ^= cpu_registers[register2];
                break;

            case 0x04: {
                uint8_t new_regval = cpu_registers[register1] + cpu_registers[register2];
                // Check for overflow
                cpu_registers[NUM_CPU_REGISTERS-1] = new_regval < cpu_registers[register1] ? 1 : 0;
                cpu_registers[register1] = new_regval;
                break;
            }

            case 0x05: {
                uint8_t new_regval = cpu_registers[register1] - cpu_registers[register2];
                // Check for overflow
                cpu_registers[NUM_CPU_REGISTERS-1] = new_regval > cpu_registers[register1] ? 0 : 1;
                cpu_registers[register1] = new_regval;
                break;
            }

            case 0x06: {
                // Store least significant bit of Vx
                cpu_registers[NUM_CPU_REGISTERS-1] = cpu_registers[register1] & 1; 
                cpu_registers[register1] >>=1;
                break;
            }

            case 0x07: {
                cpu_registers[NUM_CPU_REGISTERS-1] = cpu_registers[register1] < cpu_registers[register2] ? 0 : 1; 
                cpu_registers[register1] = cpu_registers[register2] - cpu_registers[register1];
                break;
            }

            case 0x0E: {
                // Store most significant bit of Vx
                int msb =  1 << (sizeof(uint8_t)*8);
                cpu_registers[NUM_CPU_REGISTERS-1] = (cpu_registers[register1] & msb); 
                cpu_registers[register1] <<= 1;
                break;
            }

            default:
                //TODO Throw
                break;
            }
            break;
        }

        case 0x9000: {
            if((opcode & 0x0F00) >> 8 != (opcode & 0x00F0) >> 4) {
                skip_next_instruction = true;
            }
            break;
        }

        case 0xA000:
            index_register = opcode & 0x0FFF;
            break;

        case 0xB000:
            program_counter = cpu_registers[0] + 0x0FFF;
            break;

        case 0xC000:
            cpu_registers[(opcode & 0x0F00) >> 8] = (rand()%UINT8_MAX) & (opcode & 0x00FF);
            break;

        case 0xD000: {
            bool collision = false;
            int Vx = (opcode & 0x0F00) >> 8;
            int Vy = (opcode & 0x00F0) >> 4;
            int height = opcode & 0x000F;
            for (int i = 0; i < height; i++) {
                uint8_t row = memory[index_register];
                for (int j = 0; j < 8; j++) {
                    uint8_t* pixel = &graphics_array[j+Vx][i+Vy];
                    if(*pixel == 1) {
                        collision = true;
                    }
                    int sig_bit = 1 << (sizeof(uint8_t) * j);
                    *pixel ^= sig_bit & row;
                }
            }
            cpu_registers[NUM_CPU_REGISTERS-1] = collision ? 1 : 0;
            break;
        }

        // All this key stuff is wrong
        case 0xE000: {
            switch (opcode & 0x00FF)
            {
            case 0x009E:
                skip_next_instruction = (key.getLastKey() == cpu_registers[(opcode & 0x0F00) >> 8]);
                break;

            case 0x00A1:
                skip_next_instruction = (key.getLastKey() != cpu_registers[(opcode & 0x0F00) >> 8]);
                break;
            
            default:
                // TODO throw
                break;
            }
        }

        case 0xF000: {
            uint8_t *Vx = &cpu_registers[(opcode & 0x0F00) >> 8];
            switch (opcode & 0x00FF)
            {
            case 0x0007:
                *Vx = delay_timer;
                break;
            
            case 0x000A:
                *Vx = key.waitForKeyPress();
                break;

            case 0x0015:
                delay_timer = *Vx;
                break;

            case 0x0018:
                sound_timer = *Vx;
                break;

            case 0x001E:
                index_register += *Vx;
                break;

            case 0x0029:

            default:
                break;
            }
        }
        default:
            //TODO THrow
            break;
    }



    program_counter += skip_next_instruction ? 4 : 2;
    if(program_counter > MEMORY_POOL_SIZE){
        program_counter -= MEMORY_POOL_SIZE;
    }
    fprintf(stdout, "0x%x\n", opcode);
    if(skip_next_instruction) {
        std::cout << "SKIPPING" << std::endl;
    }
}
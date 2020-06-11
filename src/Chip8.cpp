#include "Chip8.h"

Chip8::Chip8(std::string& game) 
    : program_counter(0x200) {
    
}

void Chip8::emulateCycle() {
    decodeCurrentOpcode();
    
}

void Chip8::decodeCurrentOpcode() {
    opcode = memory[program_counter];
    bool skip_next_instruction = false;
    switch(opcode & 0xF000) {
        case 0x1000:
            program_counter = opcode & 0x0FFF;
            index_register = CPU_REGISTER_MAX;
            return;
        case 0x2000:
            // Call routine at 0x2000 somehow
            break;
        case 0x3000: {
            if(memory[opcode & 0x00FF] == cpu_registers[opcode & 0x0F00]) {
                skip_next_instruction = true;
            }
            break;
        }

        case 0x4000: {
            if(memory[opcode & 0x00FF] != cpu_registers[opcode & 0x0F00]) {
                skip_next_instruction = true;
            }
            break;
        }

        case 0x5000: {
            if(cpu_registers[opcode & 0x0F00] == cpu_registers[opcode & 0x00F0]) {
                skip_next_instruction = true;
            }
            break;
        }

        case 0x6000:
            cpu_registers[opcode & 0x0F00] = opcode & 0x00FF;
            break;
        
        case 0x7000:
            cpu_registers[opcode & 0x0F00] += opcode & 0x00FF;
            break;

        case 0x8000:
            uint8_t register1 = opcode & 0x0F00;
            uint8_t register2 = opcode & 0x00F0;
            switch (opcode & 0x000F)
            {
            case 0:
                cpu_registers[register1] = cpu_registers[register2];
                break;

            case 1:
                cpu_registers[register1] |= cpu_registers[register2];
                break;

            case 2:
                cpu_registers[register1] &= cpu_registers[register2];
                break;

            case 3:
                cpu_registers[register1] ^= cpu_registers[register2];
                break;

            
            
            default:
                break;
            }

    }

    program_counter+=2;
}
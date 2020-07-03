#include "Chip8.h"
#include <iostream>

/* Fontset for chip8. E.g
DEC   HEX    BIN         RESULT    DEC   HEX    BIN         RESULT
240   0xF0   1111 0000    ****     240   0xF0   1111 0000    ****
144   0x90   1001 0000    *  *      16   0x10   0001 0000       *
144   0x90   1001 0000    *  *      32   0x20   0010 0000      *
144   0x90   1001 0000    *  *      64   0x40   0100 0000     *
240   0xF0   1111 0000    ****      64   0x40   0100 0000     *

Note that only the first 4 bytes are used to draw in each case
*/ 

uint8_t chip8_fontset[FONTSET_SIZE] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

uint8_t c = 0;
Chip8::Chip8() {
    
    program_counter = 0x200;
    opcode = 0;
    stack_pointer = 0;
    delay_timer = 0; 
    sound_timer = 0;
    index_register = 0;
    memory.fill(0);
    memset(&graphics_array, 0, GRAPHICS_ARRAY_HEIGHT*GRAPHICS_ARRAY_WIDTH*sizeof(uint8_t));
    stack.fill(0);
    cpu_registers.fill(0);

    // Load fontset
    for(int i = 0; i < FONTSET_SIZE; ++i) {
        memory[i] = chip8_fontset[i];
    }

    std::cout << "Setting up graphics" << std::endl;
    // Set up render system and register input callbacks
	graphics = std::make_unique<Graphics>();
    draw_flag = true;

    std::cout << "Setting up keyboard" << std::endl;

}

Chip8::~Chip8(){}

void Chip8::loadGame(const std::string& path){
    std::ifstream gamefile;
    gamefile.open(path.c_str());
    uint8_t ch = gamefile.get();

    uint16_t i = program_counter;

    std::cout << "Loading files..." << std::endl;

    while (gamefile.good() && i < MEMORY_POOL_SIZE) {
        memory[i++] = ch;
        ch = gamefile.get();
        // fprintf(stdout, "0x%x\n", ch);
    }
    gamefile.close();

}
void Chip8::emulateCycle() {
    processCurrentOpcode();
    updateTimers();
    if(draw_flag){
        glClear(GL_COLOR_BUFFER_BIT);
        graphics->draw(graphics_array);
        glutSwapBuffers();
        draw_flag = false;
    }
}

void Chip8::processCurrentOpcode() {
    opcode = memory[program_counter] << 8 | memory[program_counter+1];
    //LOG("0x%x", opcode);
    bool skip_next_instruction = false;
    // VF, the cpu register used to flag something
    uint8_t* flag_register = &cpu_registers[NUM_CPU_REGISTERS-1];
    switch(opcode & 0xF000) {
        case 0x0000:{
            if((opcode & 0x00F0) == 0x00E0){
                switch (opcode & 0x000F)
                {
                case 0x0000:
                    // Clear screen
                    memset(graphics_array, 0, GRAPHICS_ARRAY_HEIGHT*GRAPHICS_ARRAY_WIDTH*sizeof(uint8_t));
                    draw_flag = true;
                    break;

                case 0x000E:
                    --stack_pointer;
                    program_counter = stack[stack_pointer];
                    break;

                default:
                    //Throw
                    break;
                }
            }
            else{
                //Throw;
            }
            break;
        }

        case 0x1000:
            program_counter = opcode & 0x0FFF;
            return; //return to avoid incrementing program_counter
        case 0x2000:
            stack[stack_pointer] = program_counter;
            ++stack_pointer;
            program_counter = opcode & 0x0FFF;
            return;
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
                *flag_register = new_regval < cpu_registers[register1] ? 1 : 0;
                cpu_registers[register1] = new_regval;
                break;
            }

            case 0x05: {
                uint8_t new_regval = cpu_registers[register1] - cpu_registers[register2];
                // Check for overflow
                *flag_register = new_regval > cpu_registers[register1] ? 0 : 1;
                cpu_registers[register1] = new_regval;
                break;
            }

            case 0x06: {
                // Store least significant bit of Vx
                *flag_register = cpu_registers[register1] & 1; 
                cpu_registers[register1] >>=1;
                break;
            }

            case 0x07: {
                *flag_register = cpu_registers[register1] < cpu_registers[register2] ? 0 : 1; 
                cpu_registers[register1] = cpu_registers[register2] - cpu_registers[register1];
                break;
            }

            case 0x0E: {
                // Store most significant bit of Vx
                int msb =  1 << (sizeof(uint8_t)*8);
                *flag_register = (cpu_registers[register1] & msb); 
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
            if(index_register > MEMORY_POOL_SIZE){
                ;//throw - this will never happen
            }
            break;

        case 0xB000:
            program_counter = cpu_registers[0] + 0x0FFF;
            return;

        case 0xC000:
            cpu_registers[(opcode & 0x0F00) >> 8] = (rand()%UINT8_MAX) & (opcode & 0x00FF);
            break;

        case 0xD000: {
            *flag_register = 0;
            int Vx = cpu_registers[(opcode & 0x0F00) >> 8];
            int Vy = cpu_registers[(opcode & 0x00F0) >> 4];
            //LOG("Drawing at (%d,%d)", Vx, Vy);
            int height = opcode & 0x000F;
            for (int i = 0; i < height; i++) {
                // The row in memory the sprite is at
                uint8_t row = memory[index_register + i];
                for (int j = 0; j < 8; j++) {
                    // 0x80 is 11111111, so this scans row bit by bit to decide to write or not
                    if(row & (0x80 >> j)){
                        uint8_t* pixel = &graphics_array[(j+Vx) + GRAPHICS_ARRAY_WIDTH*(i+Vy)];
                        if(*pixel == 1) {
                            *flag_register = 1;
                        }
                        *pixel ^= 1;
                    }
                }
            }
            draw_flag = true;
            break;
        }

        case 0xE000: {
           // uint8_t thing = ((opcode & 0x0F00) >> 8);
            // LOG("%x", thing);
            // LOG("%u", cpu_registers[(opcode & 0x0F00) >> 8]);
           // std::cout << thing << std::endl;
            switch (opcode & 0x00FF)
            {
            case 0x009E:
                skip_next_instruction = key_handler.isPressed(cpu_registers[(opcode & 0x0F00) >> 8]);
                break;

            case 0x00A1:
                skip_next_instruction = !key_handler.isPressed(cpu_registers[(opcode & 0x0F00) >> 8]);
                break;
            
            default:
                // TODO throw
                break;
            }
            break;
        }

        case 0xF000: {
            uint8_t byte2 = (opcode & 0x0F00) >> 8;
            uint8_t *Vx = &cpu_registers[byte2];
            switch (opcode & 0x00FF)
            {
            case 0x0007:
                *Vx = delay_timer;
                break;
            
            case 0x000A:
                *Vx = key_handler.waitForKeyPress();
                break;

            case 0x0015:
                delay_timer = *Vx;
                break;

            case 0x0018:
                sound_timer = *Vx;
                break;

            case 0x001E: {
                index_register += *Vx;
                if(index_register >= MEMORY_POOL_SIZE){
                //    index_register-=MEMORY_POOL_SIZE;
                    *flag_register = 1;
                }
                else{
                    *flag_register = 0;
                }
                break;
            }

            case 0x0029:
                index_register = getCharLocation(byte2);
                break;

            case 0x0033: {
                uint8_t Vx = cpu_registers[byte2];
                memory[index_register] = Vx/100;
                memory[index_register+1] = (Vx/10) % 10;
                memory[index_register+2] = (Vx%100) % 10;
                break;
            }
            case 0x0055: {
                // Dump values from V0 to Vx inclusive into memory, where x = (opcode & 0x0F00) >> 8)
                std::copy(cpu_registers.begin(), &cpu_registers[(byte2)+1], &memory[index_register]);
                break;
            }
            case 0x0065: {
                // Load values from V0 to Vx inclusive from memory, where x = (opcode & 0x0F00) >> 8)
                std::copy(&memory[index_register], &memory[index_register + (byte2)+1], cpu_registers.begin());
                break;
            }
            default:
                //TODO : Throw
                break;
            }
            break;
        }
        default:
            //TODO THrow
            break;
    }

    program_counter += skip_next_instruction ? 4 : 2;
    if(program_counter >= MEMORY_POOL_SIZE){
        program_counter -= MEMORY_POOL_SIZE;
    }
}

uint16_t Chip8::getCharLocation(uint8_t character) {
    if(character > 0x0F){
        //Throw
    }

    return cpu_registers[character] * 5;
}

void Chip8::updateTimers(){
      // Update timers
    if(delay_timer > 0) {
        --delay_timer;
    }

    if(sound_timer > 0){
        if(sound_timer == 1)
        printf("BEEP!\n");
        --sound_timer;
    }  
}

void Chip8::reshapeWindow(GLsizei w, GLsizei h){
    graphics->reshapeWindow(w,h);
}

void Chip8::keyboardDown(unsigned char key, int x, int y){
    LOG("%c", key);
    key_handler.keyboardDown(key);
}
void Chip8::keyboardUp(unsigned char key, int x, int y){
    key_handler.keyboardUp(key);
}
#pragma once
#include <vector>
#include <stdint.h>
#include <array>
#include <map>
#include "Logger.h"


#define NUMBER_OF_KEYS 16

class Key {
public:
    Key();
    bool isPressed(uint8_t key);
    uint8_t waitForKeyPress();

    void keyboardDown(unsigned char key);
    void keyboardUp(unsigned char key);

private:
    const std::map<char, uint8_t> keymap = {
        std::make_pair('0', 0x00),
        std::make_pair('w', 0x01),
        std::make_pair('2', 0x02),
        std::make_pair('3', 0x03),
        std::make_pair('s', 0x04),
        std::make_pair('5', 0x05),
        std::make_pair('6', 0x06),
        std::make_pair('7', 0x07),
        std::make_pair('8', 0x08),
        std::make_pair('9', 0x09),
        std::make_pair('a', 0x0A),
        std::make_pair('b', 0x0B),
        std::make_pair('i', 0x0C),
        std::make_pair('k', 0x0D),
        std::make_pair('j', 0x0E),
        std::make_pair('f', 0x0F),
    };
    std::array<bool, NUMBER_OF_KEYS> keypresses;
};
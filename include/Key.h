#pragma once
#include <vector>
#include <stdint.h>
#include <array>

#define NUMBER_OF_KEYS 16

class Key {
public:
    Key();
    bool isPressed(uint8_t key);
    uint8_t waitForKeyPress();

private:
    std::array<bool, NUMBER_OF_KEYS> keyset;
};
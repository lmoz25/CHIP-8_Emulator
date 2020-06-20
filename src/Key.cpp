#include "Key.h"

Key::Key() {
    keyset.fill(false);
}

bool Key::isPressed(uint8_t key) {
    if(key >= NUMBER_OF_KEYS) {
        // TODO: Maybe throw?
        return false;
    }
    return keyset[key];
}

uint8_t Key::waitForKeyPress() {
    // TODO
    return 0;
}
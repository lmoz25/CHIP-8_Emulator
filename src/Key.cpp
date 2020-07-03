#include "Key.h"

Key::Key() {
    keypresses.fill(false);
}

bool Key::isPressed(uint8_t key) {
    if(key >= NUMBER_OF_KEYS) {
        LOG("%c yes", key);
        // TODO: Maybe throw?
        return false;
    }
    return keypresses[key];
}

uint8_t Key::waitForKeyPress() {
    uint8_t i = 0;
    while(true){
        if(keypresses[i]){
            break;
        }
        i++;
        if(i > NUMBER_OF_KEYS){
            i = 0;
        }
    }
    return i;
}


void Key::keyboardDown(unsigned char key){
    auto keeey = std::string(reinterpret_cast<char*>(&key));
    auto pressed_key = keymap.find(key);

    if( pressed_key != keymap.end() ){
        LOG("%s", keeey.c_str());
        keypresses[pressed_key->second] = true;
    } 
}
void Key::keyboardUp(unsigned char key){
    auto pressed_key = keymap.find(key);
    if( pressed_key != keymap.end() ){
        keypresses[pressed_key->second] = false;
    } 
}
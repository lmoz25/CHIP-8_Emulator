#include "Chip8.h"

int main() {
	std::string gamepath("");
	Chip8 chip8(gamepath);
	 // Set up render system and register input callbacks
	setupGraphics();
	setupInput();
 
	// Emulation loop
	while(1) {
		// Emulate one cycle
		chip8.emulateCycle();
 
		// If the draw flag is set, update the screen
		if(chip8.drawFlag)
	  	drawGraphics();
 
		// Store key press state (Press and Release)
		chip8.setKeys();	
  	}
 
	return 0;
}
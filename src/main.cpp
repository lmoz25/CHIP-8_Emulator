#include "Chip8.h"

int main() {
	std::string gamepath("/home/liam/Downloads/15PUZZLE");
	Chip8 chip8(gamepath);
	 // Set up render system and register input callbacks
	//setupGraphics();
	//setupInput();
	int i = 0;
	// Emulation loop
	while(i<100) {
		// Emulate one cycle
		chip8.emulateCycle();
		i++;
		// If the draw flag is set, update the screen
		//if(chip8.draw_flag)
	  //	drawGraphics();
 
		// Store key press state (Press and Release)
		//chip8.setKeys();
  	}
 
	return 0;
}
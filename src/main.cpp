#include "Chip8.h"
#include <cstdio>

Chip8 chip8;
static void displayCallback();
static void reshapeCallback(GLsizei w, GLsizei h);
static void keyboardUpCallback(unsigned char key, int x, int y);
static void keyboardDownCallback(unsigned char key, int x, int y);

int main(int argc, char** argv) {
	if(argc == 1){
		std::cerr << "Usage: chip.exe path/to/rom" << std::endl;
		exit(1);
	}
	std::string gamepath(argv[1]);
	chip8.loadGame(gamepath);
	LOG("Loading %s", gamepath.c_str());
	glutInit(&argc, argv);          
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(640, 320);
    glutInitWindowPosition(320, 320);
	glutCreateWindow(gamepath.c_str());
	
	glutDisplayFunc(displayCallback);
	glutIdleFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);        
	glutKeyboardFunc(keyboardDownCallback);
	glutKeyboardUpFunc(keyboardUpCallback);

	glutMainLoop();
	return 0;
}

void displayCallback(){
	chip8.emulateCycle();
}

void reshapeCallback(GLsizei w, GLsizei h){
	chip8.reshapeWindow(w, h);
}

void keyboardUpCallback(unsigned char key, int x, int y){
	chip8.keyboardUp(key, x, y);
}

void keyboardDownCallback(unsigned char key, int x, int y){
	chip8.keyboardDown(key, x, y);
}

#ifndef BASICFUNCS__H
#define BASICFUNCS__H


#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 700;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void loadImage();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );




void waitUntilKeyPressed();

#endif

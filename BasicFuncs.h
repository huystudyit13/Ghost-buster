
#ifndef BASICFUNCS__H
#define BASICFUNCS__H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 700;

//Walking animation
const int ANIMATION_FRAMES = 7;


bool init();
void loadImage();
bool loadMedia();
//Frees media and shuts down SDL
void close();

void waitUntilKeyPressed();

struct LTexture
{
	//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;

		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path);

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render1( int x, int y );


};

struct Input{
    int left;
    int right;
    int up;
    int down;

};
#endif

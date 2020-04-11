
#ifndef BASICFUNCS__H
#define BASICFUNCS__H


#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 700;


bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

void loadImage();


void waitUntilKeyPressed();

class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};
#endif

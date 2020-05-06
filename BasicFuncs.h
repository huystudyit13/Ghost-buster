
#ifndef BASICFUNCS__H
#define BASICFUNCS__H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

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
		bool loadFromFile( std::string path, SDL_Renderer* gRenderer);

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render1( int x, int y , SDL_Renderer* gRenderer);
};

struct Input{
    int left;
    int right;
    int up;
    int down;

};
#endif

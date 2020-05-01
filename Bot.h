#ifndef BOT__H
#define BOT__H
#include "BasicFuncs.h"
#include "Character.h"

struct Bot
{
	//The actual hardware texture
		SDL_Texture* bTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
		int x;
		int y;
		//Initializes variables
		Bot();

		//Deallocates memory
		~Bot();

		//Loads image at specified path
		bool loadBot( std::string path, SDL_Renderer* gRenderer);

		//Deallocates texture
		void free();

		//Renders texture at given point
		void renderBot(SDL_Renderer* gRenderer);
        void botmove();
        bool check(Character player);
};
#endif // BOT__H

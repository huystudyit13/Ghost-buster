#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <ctime>
#include <vector>
#include "BasicFuncs.h"
#include "Character.h"
#include "Bot.h"

LTexture gBackgroundTexture;
Character player;
vector<Bot> bot(7);

SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;
SDL_Texture* gTexture = NULL;
using namespace std;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow("Jump Student game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 255 );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}
void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

int main(int argc, char* argv[])
{
    srand(time(0));
    //Start up SDL and create window
	if( !init() )
	{
		cout << "Failed to initialize!\n" ;
	}
	else
	{
			//Main loop flag
			bool quit = false;
			//Event handler
			SDL_Event e;
            gBackgroundTexture.loadFromFile( "map1.PNG", gRenderer);
			player.load("sprite_right.PNG",gRenderer);
			player.setSpriteClip();
            for(int i=0;i<7;i++) bot[i].loadBot("ghost.PNG", gRenderer);
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					player.action(e, gRenderer);

				}
				for(int i=0;i<7;i++) if( bot[i].check(player) ) bot[i].free();
				for(int i=0;i<7;i++) bot[i].botmove();
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render texture to screen
				gBackgroundTexture.render1( 0, 0 ,gRenderer);
				player.render(gRenderer);
                for(int i=0;i<7;i++) bot[i].renderBot(gRenderer);
				//Update screen
                SDL_RenderPresent( gRenderer );

			}
	}
	waitUntilKeyPressed();
    close();
    return 0;
}





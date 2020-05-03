#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <ctime>
#include <vector>
#include <SDL_mixer.h>
#include "BasicFuncs.h"
#include "Character.h"
#include "Bot.h"
#include "musicAndTtf.h"

LTexture Background;
Character player;
Bot bot[5];
LTexture defeat;
LTexture victory;

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
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
	Mix_Quit();
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
    init();
    bool quit = false;
    SDL_Event e;
    Background.loadFromFile( "map1.PNG", gRenderer);
    defeat.loadFromFile("defeat.PNG",gRenderer);
    victory.loadFromFile("victory.PNG",gRenderer);
    player.load("sprite_right.PNG",gRenderer);
    player.setSpriteClip();
    for(int i=0;i<5;i++) bot[i].loadBot("ghost.PNG", gRenderer);
    int alive = 5;
    backgrmusic();
    while( !quit )
    {
        while( SDL_PollEvent( &e ) != 0 )
            {
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                player.action(e, gRenderer);
            }
        for(int i=0;i<5;i++){
                if( bot[i].check(player) ) {
                    bot[i].free();
                    effectmusic();
                    alive--;
                }
        }
        for(int i=0;i<5;i++) bot[i].botmove();
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        if(alive>0){
            Background.render1( 0, 0 ,gRenderer);
            player.render(gRenderer);
            for(int i=0;i<5;i++) bot[i].renderBot(gRenderer);
        }
        else if (alive==0){
            Background.render1( 0, 0 ,gRenderer);
            //defeat.render1(50,128,gRenderer);
            victory.render1(50,185,gRenderer);
        }
        SDL_RenderPresent( gRenderer );

    }
	waitUntilKeyPressed();
    close();
    return 0;
}





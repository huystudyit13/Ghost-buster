#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <ctime>
#include <vector>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "BasicFuncs.h"
#include "Character.h"
#include "Bot.h"
#include "musicAndTtf.h"


const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 700;
LTexture Background;
Character player;
LTexture defeat;
LTexture victory;
int numberOfbot = 5;
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
		gWindow = SDL_CreateWindow("Ghostbuster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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
				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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
	TTF_Quit();
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
    Bot bot[numberOfbot];
    bool quit = false;
    SDL_Event e;
    Font font;
    Font Time;
    TTF_Font *gfont = NULL;
    gfont = TTF_OpenFont( "VeraMoBd.ttf", 30 );
    SDL_Color textColor = { 255, 255, 255 };
    string first= "Press any key to play";
    font.loadFont( first, textColor ,gRenderer,gfont);
    Background.loadFromFile( "map1.PNG", gRenderer);
    defeat.loadFromFile("defeat.PNG",gRenderer);
    victory.loadFromFile("victory.PNG",gRenderer);
    player.load("sprite_right.PNG",gRenderer);
    player.setSpriteClip();
    for(int i=0;i<numberOfbot;i++) bot[i].loadBot("ghost.PNG", gRenderer);
    int alive = numberOfbot;
    backgrmusic();
    Background.render1( 0, 0 ,gRenderer);
    font.renderText(70,310,gRenderer);
    SDL_RenderPresent( gRenderer );
    waitUntilKeyPressed();
    int time = 200;
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
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        Background.render1( 0, 0 ,gRenderer);
        string tmp = to_string(time);
        Time.loadFont(tmp,textColor,gRenderer,gfont);
        if(alive>0 && time>0){
            player.render(gRenderer);
            for(int i=0;i<numberOfbot;i++) bot[i].renderBot(gRenderer);
            Time.renderText(10,0,gRenderer);
            for(int i=0;i<numberOfbot;i++){
                if( bot[i].check(player) ) {
                    bot[i].free();
                    effectmusic();
                    alive--;
                }
            }
            for(int i=0;i<numberOfbot;i++) bot[i].botmove();
        }
        else if (alive==0){
            victory.render1(50,185,gRenderer);
        }
        else if(time<1){
            defeat.render1(50,128,gRenderer);
        }
        SDL_RenderPresent( gRenderer );
        if(alive>0) time--;

    }
    close();
    return 0;
}





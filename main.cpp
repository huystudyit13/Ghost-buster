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
LTexture defeat;
LTexture victory;
LTexture openning;
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
    SDL_Event e;
    Font Time;
    TTF_Font *gfont = NULL;
    gfont = TTF_OpenFont( "font.ttf", 30 );
    SDL_Color textColor = { 255, 255, 255 };
    Background.loadFromFile( "map1.PNG", gRenderer);
    openning.loadFromFile("openning.PNG", gRenderer);
    defeat.loadFromFile("defeat.PNG",gRenderer);
    victory.loadFromFile("victory.PNG",gRenderer);
    Bot bot[numberOfbot];
    Font again;
    string Again = "Press space to play again";
    again.loadFont(Again,textColor,gRenderer,gfont);
    backgrmusic();
    bool run = true;
    do{
        Character player;
        player.load("sprite_right.PNG",gRenderer);
        int alive = numberOfbot;
        for(int i=0;i<numberOfbot;i++) bot[i].loadBot("ghost.PNG", gRenderer);
        player.setSpriteClip();
        openning.render1( 0, 0 ,gRenderer);
        SDL_RenderPresent( gRenderer );
        waitUntilKeyPressed();
        int time = 201;
        bool quit = false;
        while( !quit )
        {
            while( SDL_PollEvent( &e ) != 0 )
                {
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                        run = false;
                    }
                    player.action(e, gRenderer);
                }
            SDL_RenderClear( gRenderer );
            Background.render1( 0, 0 ,gRenderer);
            string tmp = to_string(time);
            Time.loadFont(tmp,textColor,gRenderer,gfont);
            for(int i=0;i<numberOfbot;i++) bot[i].botmove();
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
            }
            else if (alive==0){
                victory.render1(50,185,gRenderer);
                again.renderText(20 , 500 , gRenderer);
                while( SDL_PollEvent( &e ) != 0 ){
                    if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE )
                    {
                        player.free();
                        quit = true;
                        run = true;
                    }
                 }
            }
            else if(time<=0 && alive > 0 ){
                defeat.render1(50,128,gRenderer);
                again.renderText(20, 500 , gRenderer);
                 while( SDL_PollEvent( &e ) != 0 ){
                    if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE )
                    {
                        for(int i=0;i<numberOfbot;i++) bot[i].free();
                        player.free();
                        quit = true;
                        run = true;
                    }
                 }
            }
            if(alive>0) time--;
            SDL_Delay(30);
            SDL_RenderPresent( gRenderer );
            SDL_Delay(20);
        }
    }
    while(run);
    close();
    return 0;
}

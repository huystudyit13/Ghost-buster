#include "Bot.h"

#include <cstdlib>

bool Bot::loadBot( std::string path, SDL_Renderer* gRenderer)
{
	//The final texture
	SDL_Texture* newTexture = NULL;
    //Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	//Return success
	bTexture = newTexture;
	return bTexture != NULL;
}
void Bot::renderBot(SDL_Renderer* gRenderer)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy( gRenderer, bTexture, NULL, &renderQuad );
}
void Bot::botmove(){
    int n = rand() % 4 ;
    if(n==0){
        x+=40;
        if(x>453) x=453;
    }
    else if(n==1){
        x-=40;
        if(x<0) x=0;
    }
    else if(n==2){
        y-=40;
        if(y<0) y=0;
    }
    else if(n==3){
        y+=40;
        if(y>661) y=661;
    }
    SDL_Delay(20);
}

void Bot::free()
{
	//Free texture if it exists
	if( bTexture != NULL )
	{
		SDL_DestroyTexture( bTexture );
		bTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		x=0;
		y=0;
	}
}

Bot::Bot()
{
	//Initialize
	bTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	x=rand() % 453;
    y=rand() % 661;
}
Bot::~Bot()
{
	//Deallocate
	free();
}

bool Bot::check(Character player){
    int leftA = x; int leftB=player.x_pos;
    int rightA = x+mWidth; int rightB=player.x_pos+player.w_frame;
    int topA = y; int topB=player.y_pos;
    int bottomA= y + mHeight; int bottomB=player.y_pos+ player.h_frame;
    if( (( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB )) ==false  )
            {
                //A collision is detected
                return true;
            }
    return false;
}


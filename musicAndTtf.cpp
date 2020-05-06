#include "musicAndTtf.h"

using namespace std;

void backgrmusic(){
    Mix_Music* backgrmusic = NULL;
    backgrmusic = Mix_LoadMUS( "background.MP3" );
    Mix_PlayMusic(backgrmusic,-1);
}
void effectmusic(){
    Mix_Chunk* effect = NULL;
    effect = Mix_LoadWAV( "effect.MP3" );
    Mix_PlayChannel(-1,effect,0);
}

Font::Font(){
    mTexture = NULL;
	w = 0;
	h= 0;
}
Font::~Font(){
    free();
}

bool Font::loadFont( std::string &textureText, SDL_Color textColor ,SDL_Renderer* render,TTF_Font *gFont){
    //Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( render, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			w = textSurface->w;
			h = textSurface->h;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	//Return success
	return mTexture != NULL;
}
void Font::renderText( int x, int y, SDL_Renderer* render ){
	SDL_Rect renderQuad = { x, y, w, h};
	SDL_RenderCopy( render, mTexture, NULL, &renderQuad );
}
void Font::free(){
    if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		w = 0;
		h = 0;
	}
}

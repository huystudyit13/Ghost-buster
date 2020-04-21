#include "Character.h"
#include "BasicFuncs.h"

Character::Character(){
    frame = 0;
    x_pos = 240;
    y_pos = 630;
    x_val = 0;
    y_val = 0;
    w_frame = 0;
    h_frame = 0;
    status = -1;
    pTexture = NULL;
}
Character::~Character(){

};

bool Character::load(std::string path,SDL_Renderer* gRenderer){
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
			w_frame = loadedSurface->w/7;
			h_frame = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	pTexture = newTexture;
	return pTexture != NULL;
}

void Character::setSpriteClip(){
    if(w_frame > 0 && h_frame > 0){
        for(int i=0;i<ANIMATION_FRAMES;i++){
            gSpriteClips[ i ].x =   i*w_frame;
            gSpriteClips[ i ].y =  0;
            gSpriteClips[ i ].w =  w_frame;
            gSpriteClips[ i ].h = h_frame;
		}
    }
}

void Character::render(SDL_Renderer* gRenderer){
    if(status == Left){
        load("sprite_left.PNG",gRenderer);
    }
    else{
        load("sprite_right.PNG",gRenderer);
    }
    if(input.left == 1 || input.right == 1 ) frame++;
    else frame = 0;
    if(frame>=7) frame = 0 ;

    SDL_Rect* currentClip = &gSpriteClips[frame];
    SDL_Delay(100);
    SDL_Rect renderQuad = { x_pos, y_pos, w_frame, h_frame };
    if( currentClip != NULL){
        renderQuad.w = currentClip->w;
        renderQuad.h = currentClip->h;
    }
    SDL_RenderCopy( gRenderer, pTexture, currentClip, &renderQuad );
}

void Character::action(SDL_Event event , SDL_Renderer* gRenderer){
    if(event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym){
            case SDLK_RIGHT:{
                status = Right;
                input.right=1;
                input.left =0 ;
            }
            break;
            case SDLK_LEFT:{
                status = Left;
                input.left=1;
                input.right=0;
            }
            break;
        }
    }
    else if(event.type == SDL_KEYUP){
        switch(event.key.keysym.sym){
            case SDLK_RIGHT:{
                input.right=0;
            }
            break;
            case SDLK_LEFT:{
                input.left=0;
            }
            break;
        }
    }
}
void Character::free()
{
	//Free texture if it exists
	if( pTexture != NULL )
	{
		SDL_DestroyTexture( pTexture );
		frame = 0;
        x_pos = 240;
        y_pos = 630;
        x_val = 0;
        y_val = 0;
        w_frame = 0;
        h_frame = 0;
        status = -1;
        pTexture = NULL;
	}
}


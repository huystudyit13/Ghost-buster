#include "Character.h"
#include "BasicFuncs.h"

Character::Character(){
    frame = 0;
    x_pos = 240;
    y_pos = 630;
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
    if(input.left == 1 || input.right ==1) frame++;
    else frame = 0;

    if(frame>=3 || input.up==1 || input.down ==1) frame = 0 ;

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

                moveright();
            }
            break;
            case SDLK_LEFT:{
                status = Left;
                input.left=1;
                input.right=0;
                moveleft();
            }
            break;
            case SDLK_UP:{
                input.up=1;
                up();
            }
            break;
            case SDLK_DOWN:{
                input.down=1;
                down();
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
            case SDLK_UP:{
                input.up=0;
            }
            case SDLK_DOWN:{
                input.down=0;
            }
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
        w_frame = 0;
        h_frame = 0;
        status = -1;
        pTexture = NULL;
	}
}
void Character::moveleft(){
    x_pos -= 20;
    if(x_pos<135 && ((y_pos<664 && y_pos>520) || (y_pos+h_frame<664 && y_pos+h_frame>525))) x_pos=135;
    else if((x_pos>190&& x_pos<304) && ((y_pos<430 && y_pos>371) || (y_pos+h_frame<430 && y_pos+h_frame>371))) x_pos=304;
    else if((x_pos>308&& x_pos<409) && ((y_pos<310 && y_pos>276) || (y_pos+h_frame<310 && y_pos+h_frame>276))) x_pos=409;
    else if((x_pos>265&& x_pos<341) && ((y_pos<210 && y_pos>187) || (y_pos+h_frame<210 && y_pos+h_frame>187))) x_pos=341;
    else if((x_pos>123&& x_pos<198) && ((y_pos<150 && y_pos>98) || (y_pos+h_frame<150 && y_pos+h_frame>98))) x_pos=198;
    else if(x_pos<82 && ((y_pos<150 && y_pos>77) || (y_pos+h_frame<150 && y_pos+h_frame>77))) x_pos=82;
    else if(x_pos<5) x_pos=5;
    cout << x_pos << ' ' << y_pos << endl;
}
void Character::moveright(){
    x_pos +=20;
    if(x_pos+w_frame>365 && ((y_pos<664 && y_pos>520) || (y_pos+h_frame<664 && y_pos+h_frame>525))) x_pos=365-w_frame+10;
    else if((x_pos+w_frame>190 && x_pos+w_frame<304) && ((y_pos<430 && y_pos>371) || (y_pos+h_frame<430 && y_pos+h_frame>371))) x_pos=190-w_frame+10;
    else if((x_pos+w_frame>308 && x_pos+w_frame<409) && ((y_pos<310 && y_pos>276) || (y_pos+h_frame<310 && y_pos+h_frame>276))) x_pos=308-w_frame+10;
    else if((x_pos+w_frame>265 && x_pos+w_frame<341) && ((y_pos<210 && y_pos>187) || (y_pos+h_frame<210 && y_pos+h_frame>187))) x_pos=265-w_frame+10;
    else if((x_pos+w_frame>123 && x_pos+w_frame<198) && ((y_pos<150 && y_pos>98) || (y_pos+h_frame<150 && y_pos+h_frame>98))) x_pos=123-w_frame+10;
    else if(x_pos+w_frame>424 && ((y_pos<221 && y_pos>188) || (y_pos+h_frame<221 && y_pos+h_frame>188))) x_pos=424-w_frame+10;
    else if(x_pos>(500-w_frame)) x_pos=500-w_frame+7;
    cout << x_pos << ' ' << y_pos << endl;
}
void Character::up(){
    y_pos -=20;
    cout << x_pos << ' ' << y_pos << endl;
}
void Character::down(){
    y_pos +=20;
    cout << x_pos << ' ' << y_pos << endl;
}

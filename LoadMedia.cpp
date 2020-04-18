#include <iostream>
#include "BasicFuncs.h"
using namespace std;

LTexture gSpriteSheetTexture;
LTexture gBackgroundTexture;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;

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

bool loadMedia()
{
	bool success = true;

	if( !gSpriteSheetTexture.loadFromFile( "sprite.png" ) )
	{
		success = false;
	}
	else
	{
		//Set sprite clips
		for(int i=0;i<WALKING_ANIMATION_FRAMES;i++){
            gSpriteClips[ i ].x =   i*42;
            gSpriteClips[ i ].y =  0;
            gSpriteClips[ i ].w =   42;
            gSpriteClips[ i ].h = 39;
		}
	}

	if( !gBackgroundTexture.loadFromFile( "map1.png" ) )
	{
		success = false;
	}


	return success;
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

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
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::render1( int x, int y )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

void loadImage()
{
    //Start up SDL and create window
	if( !init() )
	{
		cout << "Failed to initialize!\n" ;
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			cout <<  "Failed to load media!\n" ;
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			//Current animation frame
			int frame = 0;

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
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render texture to screen
				gBackgroundTexture.render1( 0, 0 );

				//Render current frame
                SDL_Rect* currentClip = &gSpriteClips[ frame / 7 ];
                SDL_Delay(10);
                gSpriteSheetTexture.render( 240, 630, currentClip );

				//Update screen
                SDL_RenderPresent( gRenderer );

				//Go to next frame
                ++frame;

				//Cycle animation
				if( frame / 7 >= WALKING_ANIMATION_FRAMES )
				{
					frame = 0;
				}
			}
		}
	}
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

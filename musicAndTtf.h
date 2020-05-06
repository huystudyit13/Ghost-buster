#ifndef MUSICANDTTF__H
#define MUSICANDTTF__H
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "BasicFuncs.h"

void backgrmusic();
void effectmusic();

struct Font
{
    //The actual hardware texture
    SDL_Texture* mTexture;
    int w;
    int h;
    Font();
    ~Font();

    //Creates image from font string
    bool loadFont( std::string &textureText, SDL_Color textColor ,SDL_Renderer* render,TTF_Font *gFont);

    //Deallocates texture
    void free();

    //Renders texture at given point
    void renderText( int x, int y, SDL_Renderer* render);

};

#endif // MUSICANDTTF__H

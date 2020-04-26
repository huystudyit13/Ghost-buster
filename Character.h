#ifndef CHARACTER__H
#define CHARACTER__H

#include "BasicFuncs.h"
struct Character : LTexture{

    int x_pos;
    int y_pos;

    int w_frame;
    int h_frame;
    SDL_Rect gSpriteClips[ ANIMATION_FRAMES ];

    Input input;
    int frame;
    int status;
    SDL_Texture* pTexture;

    Character();
    ~Character();
    enum type{
        Right = 0,
        Left = 1,
    };

    bool load(std::string path,SDL_Renderer* gRenderer);
    void render(SDL_Renderer* gRenderer);
    void action(SDL_Event event , SDL_Renderer* gRenderer);
    void setSpriteClip();
    void free();
    void moveleft();
    void moveright();
    void up();
    void down();
};







#endif // CHARACTER__H

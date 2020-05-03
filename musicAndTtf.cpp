#include "musicAndTtf.h"
#include<iostream>
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

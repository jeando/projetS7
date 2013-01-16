#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Score.h"
#include "Map.h"
class Menu;

enum events_audio {SON_HAUT, SON_BAS, SON_GAUCHE, SON_DROITE};

class Game
{
    public:
        Game(SDL_Surface* scre);
        ~Game();
        void start();
        void pause();
        bool gestionSDL_Event();
        bool gestion_Audio();

    private:
        SDL_Surface* screen;
        Menu* menu;
        Map map;
        Score score;
};

#endif // GAME_H_INCLUDED

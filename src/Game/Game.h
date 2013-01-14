#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Score.h"
#include "Map.h"
class Menu;

class Game
{
    public:
        Game();
        ~Game();
        void start();
        void pause();
        void gestion();

    private:
        Menu* menu;
        Map* map;
        Score score;
};

#endif // GAME_H_INCLUDED

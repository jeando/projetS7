#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../MyAL.h"
#include "Score.h"
#include "Map.h"
#include"Joueur.h"

class Menu;


//enum events_audio {SON_HAUT, SON_BAS, SON_GAUCHE, SON_DROITE};

class Game
{
    public:
        Game(SDL_Surface* scre, AL_Stream_Capture* _alsc,Joueur& _joueur);
        ~Game();
        void start();
        void pause();
        bool gestionSDL_Event();
        bool gestion_Audio();

    private:
        SDL_Surface* screen;
	AL_Stream_Capture* alsc;
	Joueur* _joueur;
        Menu* menu;
        Map map;
        Score score;
};

#endif // GAME_H_INCLUDED

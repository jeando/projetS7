#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include"Joueur.h"
#include "Game.h"

#include <vector>

class Choix_Utilisateur
{
    public:
        Choix_Utilisateur(SDL_Surface* scre);
		~Choix_Utilisateur();
        bool gestion_clic();
        void start();
        void draw();
        void update();


    private:
		SDL_Surface* screen;
        SDL_Surface* fene_menu;
        std::vector<std::string> list_util;
};

class Menu
{
    public:
        Menu(SDL_Surface* scre);
        ~Menu();
        bool gestion_clic();
        void start();
        void draw();
        void update();

    private:
        AL_Stream_Capture alsc;
        //MenuItem* menu_item;
        SDL_Surface* screen;
        SDL_Surface* fene_menu;
        Joueur* joueur;
        SDL_Rect rect1;
        SDL_Rect rect2;
        SDL_Rect rect3;
        SDL_Rect rect4;
};

#endif // MENU_H_INCLUDED

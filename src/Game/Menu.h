#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Game.h"

class Action
{
    public:
        Action();

        void commencer(Action prec);
        void choix_utilisateur(Action prec);
        void options(Action prec);
        void quitter(Action prec);
        void retour(Action prec);

        void choisir_map();
        void modif_audio();
        void modif_video();
        void re_enregister();
        void creer_nouvel_utilisateur();
        void modif_touches();


    private:
        Action* precedente;
};

class MenuItem
{
    public:
        MenuItem();

    private:
        SDL_Surface* surface;
        SDL_Rect rect;
        Action* action;
};

class Menu //: MenuItem
{
    public:
        Menu(SDL_Surface* scre);
        bool gestion_clic();
        void start();
        void draw();
        void update();

    private:
	AL_Stream_Capture alsc;
        Game game;
        //MenuItem* menu_item;
        SDL_Surface* screen;
        SDL_Surface* fene_menu;
        SDL_Rect rect1;
        SDL_Rect rect2;
        SDL_Rect rect3;
        SDL_Rect rect4;
};

#endif // MENU_H_INCLUDED

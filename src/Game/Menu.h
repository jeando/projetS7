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
        void retrour(Action prec);

        void choisir_map();
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

class Menu : MenuItem
{
    public:
        Menu();

    private:
        Game game;
        MenuItem* menu_item;
};

#endif // MENU_H_INCLUDED

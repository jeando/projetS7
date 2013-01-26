#ifndef MENU_OPTION_INCLUDED
#define MENU_OPTION_INCLUDED

#include"Joueur.h"
#include "Game.h"
#include<SDL/SDL_ttf.h>
#include <vector>

class Menu_option
{
    public:
        Menu_option(SDL_Surface* scre);
		virtual ~Menu_option();
        bool gestion_clic();
        void start();
        void draw();
        void update();

    private:
		SDL_Surface* screen;
        SDL_Surface* fene_menu;
    	TTF_Font* police;
        int incr_gch;
        int incr_dte;
        std::vector<std::string> capt_devices;
        std::vector<std::string> out_devices;
};


#endif // MENU_OPTION_INCLUDED

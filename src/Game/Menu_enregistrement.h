#ifndef MENU_ENREGISTREMENT_H_INCLUDED
#define MENU_ENREGISTREMENT_H_INCLUDED

#include"Joueur.h"
#include "Game.h"
#include<SDL/SDL_ttf.h>
#include <vector>
//#include<map>

class Menu_enregistrement
{
    public:
        Menu_enregistrement(SDL_Surface* scre, std::vector<std::string> list, int ind);
		virtual ~Menu_enregistrement();
        bool gestion_clic();
        void start();
        void draw();
        void update();
        bool all_enreg();
        bool is_readable( const std::string & file);

    private:
		SDL_Surface* screen;
        SDL_Surface* fene_menu;
    	TTF_Font* police;
        std::vector<std::string> list_util;
		int index;
        std::string nom_temp;
        std::vector<std::string> nom_sond;

};

#endif // MENU_ENREGISTREMENT_H_INCLUDED

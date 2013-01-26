#ifndef CHOIX_UTILISATEUR_H_INCLUDED
#define CHOIX_UTILISATEUR_H_INCLUDED

#include"Joueur.h"
#include "Game.h"
#include<SDL/SDL_ttf.h>
#include <vector>

class Choix_Utilisateur
{
    public:
        Choix_Utilisateur(SDL_Surface* scre);
		virtual ~Choix_Utilisateur();
        bool gestion_clic();
        void start();
        void draw();
        void update();
        bool all_enreg(unsigned int index);
        bool is_readable( const std::string & file);


    private:
		SDL_Surface* screen;
        SDL_Surface* fene_menu;
    	TTF_Font* police;
        std::vector<std::string> list_util;
        //std::map<int,std::string> list_util;
        std::vector<SDL_Surface*> list_util_sdl;
        int incr;
};


#endif // CHOIX_UTILISATEUR_H_INCLUDED

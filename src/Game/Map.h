#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include <map>

#include "Item.h"
#include "Frog.h"
#include"../MyAL.h"

class Map
{
    public:
        Map(std::string which_map, SDL_Surface* screen, AL_Stream_Capture* alcs);
        ~Map();
        void load_map(std::string nom);
        void save_map(std::string nom);

        //on redessine tout (ne pas oublier croa_croa!)
        void draw();
        //on dessine sur la grenouille le fond et on dessine la grenouille sur la nouvelle position
        void update();//, unsigned int x, unsigned int y);
        bool change_speed(int vx, int vy);
        bool is_deplacement_possible(int vx, int vy);
	bool victoire();


    private:
        SDL_Surface* screen;
        SDL_Surface* surface;
		AL_Stream_Capture* alsc;
        unsigned int size_box_x;
        unsigned int size_box_y;
        unsigned int w_map;
        unsigned int h_map;
        std::vector<Item*> list_items;
        Frog croa_croa;
        std::map<std::string,SDL_Surface*> surfaces_map;
	unsigned int cpt_position;
	int coord;
};
#endif // MAP_H_INCLUDED

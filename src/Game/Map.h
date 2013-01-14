#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <SDL/SDL.h>
#include <iostream>
#include <vector>

#include "Item.h"
#include "Frog.h"


class Map
{
    public:
        Map(std::string which_map);
        ~Map();
        void load_map(std::string nom);
        void save_map(std::string nom);
        //on redessine tout (ne pas oublier croa_croa!)
        void draw(SDL_Surface* screen);
        //on dessine sur la grenouille le fond et on dessine la grenouille sur la nouvelle position
        void update(SDL_Surface* screen);//, unsigned int x, unsigned int y);
        bool change_speed(int vx, int vy);


    private:
        SDL_Suface* my_map;
        unsigned int size_box_x;
        unsigned int size_box_y;
        unsigned int w_map;
        std::vector<Item*> list_items;
        Frog croa_croa;
};
#endif // MAP_H_INCLUDED

#ifndef sdl_h_projet_son
#define sdl_h_projet_son

#include<SDL/SDL.h>
#include<SDL/SDL_gfxPrimitives.h>
#include<vector>
template<typename T>
void draw_vect(SDL_Surface* screen, std::vector<T> vect);
template<typename T>
void draw_mat(SDL_Surface* screen, std::vector<std::vector<T> > vect);

#include"sdl.cpp"
#endif


#ifndef sdl_h_projet_son
#define sdl_h_projet_son

#include<SDL/SDL.h>
#include<SDL/SDL_gfxPrimitives.h>
#include<vector>
template<typename T>
void draw_vect(SDL_Surface* screen, std::vector<T> vect);
template<typename T>
void draw_vect(SDL_Surface* screen, std::vector<T> vect, unsigned int size_h);
template<typename T>
void draw_vect(SDL_Surface* screen, std::vector<T> vect, unsigned int size_h, unsigned int size_x_for_one);
template<typename T>
void draw_mat(SDL_Surface* screen, std::vector<std::vector<T> > vect,
		unsigned int size_x=1, unsigned int size_y=1);
template<typename T>
void draw_spectro_3D(SDL_Surface* screen,
		std::vector<std::vector<T> > spectro,
		unsigned int h_max, double theta, unsigned int space_x);
#include"sdl.cpp"
#endif


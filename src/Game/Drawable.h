#ifndef _DRAWABLE_H
#define _DRAWABLE_H
#include <iostream>
#include <SDL/SDL.h>
#include <cstdlib>
#include <SDL/SDL_image.h>

class Drawable {
    public:
        Drawable(std::string img);
        virtual ~Drawable();
        virtual void draw(SDL_Surface* screen);
        void set_image(std::string img);

    protected:
        SDL_Surface* my_image;

};
#endif

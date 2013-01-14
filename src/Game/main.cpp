#include "Drawable.h"

int main()
{
        //configuration de la fenetre
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Surface * screen(SDL_SetVideoMode(1520, 840, 32, SDL_HWSURFACE));

        Drawable dr("frog.png");
        dr.draw(screen);


        sleep(10);
        SDL_Quit();
}

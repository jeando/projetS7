#include "Map.h"

int main()
{
        //configuration de la fenetre
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Surface * screen(SDL_SetVideoMode(1520, 840, 32, SDL_HWSURFACE));

        Map m("map1",screen);
        m.draw(screen);

        for(int i=0; i<30; i++)
        {
            m.update(screen);
            system("sleep 0.3");
        }

		system("sleep 10");
        SDL_Quit();
}

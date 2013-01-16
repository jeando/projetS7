#include "Game.h"

int main()
{
        //configuration de la fenetre
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Surface * screen(SDL_SetVideoMode(1501, 811, 32, SDL_HWSURFACE));
        SDL_WM_SetIcon(IMG_Load("../../images/icone.png"), NULL);

        Game g(screen);
        g.start();

        SDL_Quit();
}

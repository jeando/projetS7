#include "Menu.h"

int main()
{
        //configuration de la fenetre
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Surface * screen(SDL_SetVideoMode(1441, 751, 32, SDL_HWSURFACE));
        SDL_WM_SetIcon(SDL_LoadBMP("../../images/icone.bmp"), NULL);

		AL_Stream_Capture alsc;
		Joueur joueur("moi");
        Game g(screen, &alsc, joueur);
        g.start();

//        Menu m(screen);
//        m.start();

        SDL_Quit();
}

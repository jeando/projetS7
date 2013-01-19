#include "Menu.h"

int main()
{
        //configuration de la fenetre
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Surface * screen(SDL_SetVideoMode(1441, 751, 32, SDL_HWSURFACE|SDL_DOUBLEBUF));
        SDL_WM_SetIcon(SDL_LoadBMP("../../images/icone.bmp"), NULL);

		AL_Stream_Capture alsc;
		Joueur joueur("moi");
//        Game g(screen, &alsc, joueur);
  //      g.start();

       Menu* m = new Menu(screen);
        m->start();
	std::cout << "aaaaaaaaaaaa" << std::endl;
		delete m;
	std::cout << "aaaaaaaaaaaa" << std::endl;
        SDL_Quit();
}

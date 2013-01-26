#include "Menu.h"
#include "Menu_Option.h"
#include "Choix_Utilisateur.h"
#include"Game.h"
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;
extern AL_Stream_Capture alsc;
//extern AL_Stream_Capture_And_Play alsc;

class ini_ttf
{
	public:
		ini_ttf()
		{
			TTF_Init();
		}
		~ini_ttf()
		{
			TTF_Quit();
		}
}ini_ttf;


Menu::Menu(SDL_Surface* scre)
:
	screen(SDL_SetVideoMode(750, 400, scre->format->BitsPerPixel, scre->flags)),
	fene_menu(SDL_CreateRGBSurface(screen->flags, screen->w,
    screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,	screen->format->Amask)),
	police(TTF_OpenFont("./data/Arial.ttf", 35)),
	joueur(nullptr)
{
    draw();
    update();
}

Menu::~Menu()
{
	delete joueur;
    SDL_FreeSurface(fene_menu);
	TTF_CloseFont(police);
}
void Menu::draw()
{
    SDL_WM_SetCaption("Menu", nullptr);
    SDL_FillRect(fene_menu, nullptr, SDL_MapRGB(fene_menu->format, 17, 206, 112));

    SDL_Surface* commencer = IMG_Load( "../../images/buttun_begin.png" );
    SDL_Surface* standard = IMG_Load( "../../images/buttun.png" );
    SDL_Surface* quitter = IMG_Load( "../../images/buttun_quit.png" );

    SDL_Color couleur = {0, 0, 0, 42};

    //positionnement et affichage des boutons
    /*rect1.x=620;
    rect1.y=20;
    SDL_BlitSurface(commencer,nullptr,fene_menu,&rect1);
    ostringstream oss;
    oss << " Commencer";
    SDL_Surface* texte = TTF_RenderText_Blended(police, oss.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);*/

    rect2.x=fene_menu->w*1.0/7;
    rect2.y=fene_menu->h*1.0/5;
    SDL_BlitSurface(standard,nullptr,fene_menu,&rect2);
    ostringstream oss2;
    oss2 << "   Utilisateur";
    SDL_Surface* texte = TTF_RenderText_Blended(police, oss2.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);

    rect3.x=fene_menu->w*4.0/7;
    rect3.y=fene_menu->h*1.0/5;
    SDL_BlitSurface(standard,nullptr,fene_menu,&rect3);
    ostringstream oss3;
    oss3 << "    Options";
    SDL_FreeSurface(texte);
    texte = TTF_RenderText_Blended(police, oss3.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect3);

    rect4.x=fene_menu->w/2-quitter->w/2;
    rect4.y=fene_menu->h*3.0/5;
    SDL_BlitSurface(quitter,nullptr,fene_menu,&rect4);
    ostringstream oss4;
    oss4 << "     Quitter";
    SDL_FreeSurface(texte);
    texte = TTF_RenderText_Blended(police, oss4.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect4);

	SDL_FreeSurface(texte);

    SDL_FreeSurface(commencer);
    SDL_FreeSurface(standard);
    SDL_FreeSurface(quitter);
}

void Menu::update()
{
    //SDL_Surface * screen(SDL_SetVideoMode(1441, 751, 32, SDL_HWSURFACE|SDL_DOUBLEBUF));
    SDL_BlitSurface(fene_menu,nullptr,screen,nullptr);
    SDL_Flip(screen);
}

void Menu::start()
{
    std::chrono::milliseconds dura(40);
    while(!gestion_clic())
    {
        update();
		std::this_thread::sleep_for(dura);
    }
    return;
}

bool Menu::gestion_clic()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        return true;
                        break;
        			default:
            			break;
                }
            case SDL_MOUSEBUTTONDOWN:
                {
                int x = event.button.x;
                int y = event.button.y;

                if(x>=rect2.x && x<=(rect2.x+204)
                   && y>=rect2.y && y<=(rect2.y+43))
                {
                        cout << "Choix utilisateur" << endl;
                        Choix_Utilisateur ch(screen);
                        ch.start();
						screen=SDL_SetVideoMode(fene_menu->w,
								fene_menu->h,
								fene_menu->format->BitsPerPixel ,
								fene_menu->flags);
                }
                else if(x>=rect3.x && x<=(rect3.x+204)
                   && y>=rect3.y && y<=(rect3.y+43))
                {
                        cout << "Option" << endl;
                        Menu_option mo(screen);
                        mo.start();
						screen=SDL_SetVideoMode(fene_menu->w,
								fene_menu->h,
								fene_menu->format->BitsPerPixel ,
								fene_menu->flags);
                }
                else if(x>=rect4.x && x<=(rect4.x+204)
                   && y>=rect4.y && y<=(rect4.y+43))
                {
                        cout << "Quitter" << endl;
                        return true;
                }
                return false;
                break;
                }
            case SDL_QUIT:
                return true;
                break;
            default:
                break;
        }
    }
    return false;
}


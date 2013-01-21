#include "Menu.h"
#include"Game.h"
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
extern AL_Stream_Capture alsc;

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
:alsc(),
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

                /*if(x>=rect1.x && x<=(rect1.x+204)
                   && y>=rect1.y && y<=(rect1.y+43))
                {
                        cout << "commencer" << endl;
                        Game g(screen,&alsc,joueur);
                        g.start();
                }*/
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

Choix_Utilisateur::Choix_Utilisateur(SDL_Surface* scre)
//:screen(scre), fene_menu(SDL_CreateRGBSurface(scre->flags, scre->w,
:screen(SDL_SetVideoMode(750, 400, scre->format->BitsPerPixel, scre->flags
		//	SDL_HWSURFACE | SDL_DOUBLEBUF
		)), fene_menu(SDL_CreateRGBSurface(screen->flags,
		screen->w,
    screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
	screen->format->Amask)),
		police(TTF_OpenFont("data/Arial.ttf", 25)),
		incr(0)
{
    //screen = SDL_SetVideoMode(750, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_Color couleur = {0, 0, 0, 42};
    ifstream liste_utilisateurs("./data/joueur.lst");
    string tmp;
    do
    {
      liste_utilisateurs>>tmp;
      if(tmp=="")
      {
          break;
      }
      list_util.push_back(tmp);
      list_util_sdl.push_back(TTF_RenderText_Blended(police, tmp.c_str(), couleur));
      tmp="";
    }while(true);
    liste_utilisateurs.close();
    draw();
    update();
}
Choix_Utilisateur::~Choix_Utilisateur()
{
	for(SDL_Surface* surf : list_util_sdl)
	{
		SDL_FreeSurface(surf);
	}
	SDL_FreeSurface(fene_menu);
	TTF_CloseFont(police);
}
void Choix_Utilisateur::draw()
{
    SDL_WM_SetCaption("Choix utilisateur", nullptr);
    SDL_FillRect(fene_menu, nullptr, SDL_MapRGB(fene_menu->format, 17, 206, 112));

    SDL_Surface* commencer = IMG_Load( "../../images/buttun_begin.png" );
    SDL_Surface* standard = IMG_Load( "../../images/buttun.png" );
    SDL_Surface* quitter = IMG_Load( "../../images/buttun_quit.png" );

    //TTF_Font* police = TTF_OpenFont("./data/Arial.ttf", 25);
    SDL_Color couleur = {0, 0, 0, 42};
    SDL_Surface* texte;
    SDL_Surface* texte2;

    SDL_Rect rect1={0,0,0,0};

    //texte pour changer
    ostringstream oss;
    oss << "  Re-enregistrer";
    texte = TTF_RenderText_Blended(police, oss.str().c_str(), couleur);

    //texte pour commencer
    ostringstream oss2;
    oss2 << "  Commencer";
    texte2 = TTF_RenderText_Blended(police, oss2.str().c_str(), couleur);

    for(unsigned int i=0; (i<=list_util.size() && i<4); i++)
    {
        cout << i << endl;
        //les utilisateurs
        rect1.x=25;
        rect1.y=21+i*43+i*21;
        SDL_BlitSurface(standard,nullptr,fene_menu,&rect1);

        //changer
        rect1.x=275;
        SDL_BlitSurface(standard,nullptr,fene_menu,&rect1);
        SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);

        //commencer
        rect1.x=525;
        SDL_BlitSurface(commencer,nullptr,fene_menu,&rect1);
        SDL_BlitSurface(texte2,nullptr,fene_menu,&rect1);
    }

    //bouton quitter
    rect1.x=299;
    rect1.y=325;
    SDL_BlitSurface(quitter,nullptr,fene_menu,&rect1);
    ostringstream oss4;
    oss4 << "     Quitter";
    SDL_FreeSurface(texte);
    texte = TTF_RenderText_Blended(police, oss4.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);

	//TTF_CloseFont(police);
    SDL_FreeSurface(commencer);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(texte2);
    SDL_FreeSurface(standard);
    SDL_FreeSurface(quitter);
}

void Choix_Utilisateur::update()
{
    SDL_BlitSurface(fene_menu,nullptr,screen,nullptr);

    SDL_Rect rect1={40,0,0,0};

    for(unsigned int i=incr; (i<list_util.size() && i<incr+4); i++)
    {
        rect1.y=21+(i-incr)*43+(i-incr)*21;
		SDL_BlitSurface(list_util_sdl[i],nullptr,screen,&rect1);
    }

    SDL_Flip(screen);
}

void Choix_Utilisateur::start()
{
    std::chrono::milliseconds dura(200);
    while(!gestion_clic())
    {
        update();
		std::this_thread::sleep_for(dura);
    }
    return;
}

bool Choix_Utilisateur::gestion_clic()
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
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
						{
                        int x = event.button.x;
                        int y = event.button.y;

                        for(unsigned int i=0; (i<=list_util.size() && i<4); i++)
                        {
                            int ligne = 21+i*43+i*21;
                            //re-enreg
                            if(x>=275 && x<274+204
                               && y>=ligne && y<ligne+43)
                            {
                                cout << "re-enreg" << endl;
                            }

                            //commencer
                            if(x>=525 && x<525+204
                               && y>=ligne && y<ligne+43
                               && (i+incr)<list_util.size())
                            {
                                cout << "commencer" << endl;
                                Joueur j(list_util[i+incr]);
                                Game g(screen,&alsc,j);
                                g.start();
                            }

                            //nouvel uti
                            if(x>=25 && x<25+204
                               && y>=ligne && y<ligne+43
                               && (i+incr)==list_util.size())
                            {
                                cout << "nouvel uti" << endl;
                            }

                        }

                        if(x>=299 && x<=(299+204)
                           && y>=325 && y<=(325+43))
                        {
                                cout << "Quitter" << endl;
                                return true;
                        }
						}
                        return false;
                        break;
                    case SDL_BUTTON_WHEELUP:
                        if(incr>=1)
                        {
                            incr--;
                        }
                        break;
                    case SDL_BUTTON_WHEELDOWN:
                        if(incr<list_util.size()-1)
                        {
                            incr++;
                        }
                        break;
                }
                break;
            case SDL_QUIT:
                return true;
                break;
            default:
                break;
        }
    }
    return false;
}

#include "Choix_Utilisateur.h"
#include "Menu_enregistrement.h"
#include"Game.h"
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;
extern AL_Stream_Capture alsc;
//extern AL_Stream_Capture_And_Play alsc;

Choix_Utilisateur::Choix_Utilisateur(SDL_Surface* scre)
//:screen(scre), fene_menu(SDL_CreateRGBSurface(scre->flags, scre->w,
:screen(SDL_SetVideoMode(550, 400, scre->format->BitsPerPixel, scre->flags
		//	SDL_HWSURFACE | SDL_DOUBLEBUF
		)), fene_menu(SDL_CreateRGBSurface(screen->flags,
		screen->w,
    screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
	screen->format->Amask)),
		police(TTF_OpenFont("data/Arial.ttf", 25)),
		incr(0), actif(-1)
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
        //les utilisateurs
        rect1.x=25;
        rect1.y=21+i*43+i*21;
        SDL_BlitSurface(standard,nullptr,fene_menu,&rect1);

        //changer
        rect1.x=275;
        SDL_BlitSurface(standard,nullptr,fene_menu,&rect1);
        SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);

        //commencer
        /*rect1.x=525;
        SDL_BlitSurface(commencer,nullptr,fene_menu,&rect1);
        SDL_BlitSurface(texte2,nullptr,fene_menu,&rect1);*/
    }

    rect1.y=325;
    //bouton commencer
    rect1.x=25;
    SDL_BlitSurface(commencer,nullptr,fene_menu,&rect1);
    SDL_BlitSurface(texte2,nullptr,fene_menu,&rect1);

    //bouton quitter
    rect1.x=275;
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

    for(int i=incr; (i<list_util.size() && i<incr+4); i++)
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
                            //nom utilisateur
                            if(x>=25 && x<25+204
                               && y>=ligne && y<ligne+43
                               && (i+incr)<list_util.size())
                            {
                                if(actif !=-1)
                                {
                                    SDL_Surface* act = IMG_Load("../../images/buttun.png");
                                    SDL_Rect rect1 = {0,0,204,43};
                                    SDL_Rect rect2 = {25,
                                        static_cast<Sint16>(21+(actif-incr)*(43+21)),204,43};

                                    SDL_BlitSurface(act,&rect1,fene_menu,&rect2);
                                    SDL_UpdateRect(screen, rect2.x, rect2.y, rect2.w, rect2.h);
                                    SDL_FreeSurface(act);
                                }

                                actif = i+incr;

                                SDL_Surface* act = IMG_Load("../../images/buttun_actif.png");
                                SDL_Rect rect1 = {0,0,204,43};
                                SDL_Rect rect2 = {25,
                                    static_cast<Sint16>(21+(i)*(43+21)),204,43};

                                SDL_BlitSurface(act,&rect1,fene_menu,&rect2);
                                SDL_UpdateRect(screen, rect2.x, rect2.y, rect2.w, rect2.h);
                                SDL_FreeSurface(act);
                            }

                            //re-enreg
                            if(x>=275 && x<274+204
                               && y>=ligne && y<ligne+43
                               && (i+incr)<list_util.size())
                            {
                                cout << "re-enreg" << endl;
                                Menu_enregistrement me(screen,list_util,i+incr);
                                me.start();
                                screen=SDL_SetVideoMode(fene_menu->w,
								fene_menu->h,
								fene_menu->format->BitsPerPixel ,
								fene_menu->flags);
                            }
                            //nouvel uti
                            if(x>=25 && x<25+204
                               && y>=ligne && y<ligne+43
                               && (i+incr)==list_util.size())
                            {
                                cout << "nouvel uti" << endl;
                                Menu_enregistrement me(screen,list_util,-1);
                                me.start();
                                screen=SDL_SetVideoMode(fene_menu->w,
									fene_menu->h,
									fene_menu->format->BitsPerPixel ,
									fene_menu->flags);
                            }

                        }

                        //commencer
                        if(x>=25 && x<25+204
                            && y>=325 && y<325+43)
                        {
                            if(actif!=-1 && actif<list_util.size())
                            {
                                if(all_enreg(actif))
                                {
                                    Joueur j(actif, list_util[actif]);
                                    Game g(screen,&alsc,j);
                                    g.start();
                                    screen=SDL_SetVideoMode(fene_menu->w,
                                    fene_menu->h,
                                    fene_menu->format->BitsPerPixel ,
                                    fene_menu->flags);
                                }
                                else
                                {
                                    SDL_Rect rect2 = {50,275,800,43};
                                    SDL_Surface* texte;
                                    SDL_Color couleur = {0, 0, 0, 42};
                                    SDL_FillRect(fene_menu, &rect2,
                                        SDL_MapRGB(fene_menu->format, 17, 206, 112));
                                        ostringstream oss_message;

                                    oss_message << "Veuillez enregister tous les sons";
                                    texte=TTF_RenderText_Blended(police,
                                    oss_message.str().c_str(), couleur);
                                    SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                                    SDL_FreeSurface(texte);
                                }
                            }
                            else
                            {
                                SDL_Rect rect2 = {50,275,800,43};
                                SDL_Surface* texte;
                                SDL_Color couleur = {0, 0, 0, 42};
                                SDL_FillRect(fene_menu, &rect2,
                                    SDL_MapRGB(fene_menu->format, 17, 206, 112));
                                    ostringstream oss_message;

                                oss_message << "Veuillez selectionner un utilisateur";
                                texte=TTF_RenderText_Blended(police,
                                oss_message.str().c_str(), couleur);
                                SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                                SDL_FreeSurface(texte);
                            }
                        }

                        if(x>=275 && x<=(275+204)
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

                            if(actif!=-1)
                            {
                                SDL_Surface* act = IMG_Load("../../images/buttun.png");
                                SDL_Rect rect1 = {0,0,204,43};
                                SDL_Rect rect2 = {25,
                                    static_cast<Sint16>(21+(actif-incr-1)*(43+21)),204,43};

                                SDL_BlitSurface(act,&rect1,fene_menu,&rect2);
                                SDL_UpdateRect(screen, rect2.x, rect2.y, rect2.w, rect2.h);
                                SDL_FreeSurface(act);

                                SDL_Surface* act2 = IMG_Load("../../images/buttun_actif.png");
                                SDL_Rect rect3 = {0,0,204,43};
                                SDL_Rect rect4 = {25,
                                    static_cast<Sint16>(21+(actif-incr)*(43+21)),204,43};

                                SDL_BlitSurface(act2,&rect3,fene_menu,&rect4);
                                SDL_UpdateRect(screen, rect4.x, rect4.y, rect4.w, rect4.h);
                                SDL_FreeSurface(act2);
                            }
                        }
                        break;
                    case SDL_BUTTON_WHEELDOWN:
                        if(incr<list_util.size()-1)
                        {
                            incr++;

                            if(actif!=-1)
                            {
                                SDL_Surface* act = IMG_Load("../../images/buttun.png");
                                SDL_Rect rect1 = {0,0,204,43};
                                SDL_Rect rect2 = {25,
                                    static_cast<Sint16>(21+(actif-incr+1)*(43+21)),204,43};

                                SDL_BlitSurface(act,&rect1,fene_menu,&rect2);
                                SDL_UpdateRect(screen, rect2.x, rect2.y, rect2.w, rect2.h);
                                SDL_FreeSurface(act);

                                SDL_Surface* act2 = IMG_Load("../../images/buttun_actif.png");
                                SDL_Rect rect3 = {0,0,204,43};
                                SDL_Rect rect4 = {25,
                                    static_cast<Sint16>(21+(actif-incr)*(43+21)),204,43};

                                SDL_BlitSurface(act2,&rect3,fene_menu,&rect4);
                                SDL_UpdateRect(screen, rect4.x, rect4.y, rect4.w, rect4.h);
                                SDL_FreeSurface(act2);
                            }
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

bool Choix_Utilisateur::is_readable( const string & file )
{
    ifstream fichier( file.c_str() );
    if(fichier.fail())
		return false;
	fichier.close();
	return true;
}

bool Choix_Utilisateur::all_enreg(unsigned int index)
{
    vector<string> nom_sond = {"gauche","droite","haut","bas","tir"};
    for(int i=0; i<nom_sond.size(); i++)
    {
        for(int j=0; j<2; j++)
        {
            ostringstream oss;
            oss << "./data/" << index << "_" << nom_sond[i] << "_"
                 << (j+1) << ".wav";

            if(!is_readable(oss.str().c_str()))
            {
                return false;
            }
        }
    }
    return true;
}

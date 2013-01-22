#include "Menu.h"
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

Menu_option::Menu_option(SDL_Surface* scre)
:screen(SDL_SetVideoMode(900, 600, scre->format->BitsPerPixel, scre->flags
		//	SDL_HWSURFACE | SDL_DOUBLEBUF
		)), fene_menu(SDL_CreateRGBSurface(screen->flags,
		screen->w,
    screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
	screen->format->Amask)),
		police(TTF_OpenFont("data/Arial.ttf", 25)), incr_gch(0), incr_dte(0)
{
    vector<std::string> capt = MyAL::get_capture_devices();
    vector<std::string> out = MyAL::get_devices();

    for(int i=0; i<capt.size(); i++)
    {
        istringstream iss(capt[i]);
        string tmp;
        string nom="";
        for(int i=0; i<3; i++)
        {
            getline(iss,tmp, ' ');
            if(tmp=="")
            {
                break;
            }
            nom+=" ";
            nom+=tmp;
        }
        capt_devices.push_back(nom);
    }

    for(int i=0; i<out.size(); i++)
    {
        istringstream iss(out[i]);
        string tmp;
        string nom="";
        for(int i=0; i<3; i++)
        {
            getline(iss,tmp, ' ');
            if(tmp=="")
            {
                break;
            }
            nom+=" ";
            nom+=tmp;
        }
        out_devices.push_back(nom);
    }

    draw();
    update();
}

bool Menu_option::gestion_clic()
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
                        for(int i=0; (i<6 && (i<=capt_devices.size() || i<=out_devices.size())); i++)
                        {
                            if(event.button.y>=118+i*68 && event.button.y<118+i*68+43)
                            {
                                if(event.button.x>=25 && event.button.x<25+400
                                   && (i+incr_gch<capt_devices.size()))
                                {
                                    cout << (i+incr_gch) << ": " << capt_devices[i+incr_gch] << endl;
                                }
                                else if(event.button.x>=475 && event.button.x<475+400
                                    && (i+incr_dte<out_devices.size()))
                                {
                                    cout << (i+incr_dte) << ": " << out_devices[i+incr_dte] << endl;
                                }
                            }
                        }
                        if(event.button.x>=225 && event.button.x<=(225+204)
                           && event.button.y>=532 && event.button.y<=(532+43))
                        {
                            cout << "default" << endl;
                        }
                        else if(event.button.x>=475 && event.button.x<=(475+204)
                           && event.button.y>=532 && event.button.y<=(532+43))
                        {
                                cout << "Quitter" << endl;
                                return true;
                        }
                        return false;
                        break;
						}
                    case SDL_BUTTON_WHEELUP:
                        if(incr_gch>=1 && event.button.x<250)
                        {
                            incr_gch--;
                        }
                        if(incr_dte>=1 && event.button.x>250)
                        {
                            incr_dte--;
                        }
                        break;
                    case SDL_BUTTON_WHEELDOWN:
                        if(incr_gch<capt_devices.size()-1 && event.button.x<250)
                        {
                            incr_gch++;
                        }
                        if(incr_dte<out_devices.size()-1 && event.button.x>250)
                        {
                            incr_dte++;
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

void Menu_option::start()
{
    std::chrono::milliseconds dura(200);
    while(!gestion_clic())
    {
        update();
		std::this_thread::sleep_for(dura);
    }
    return;
}

void Menu_option::draw()
{
    SDL_WM_SetCaption("Options", nullptr);
    SDL_FillRect(fene_menu, nullptr, SDL_MapRGB(fene_menu->format, 17, 206, 112));

    SDL_Surface* audio = IMG_Load( "../../images/buttun_audio.png" );
    SDL_Surface* commencer = IMG_Load( "../../images/buttun_begin.png" );
    SDL_Surface* standard = IMG_Load( "../../images/buttun.png" );
    SDL_Surface* quitter = IMG_Load( "../../images/buttun_quit.png" );

    //TTF_Font* police = TTF_OpenFont("./data/Arial.ttf", 25);
    SDL_Color couleur = {0, 0, 0, 42};
    SDL_Surface* texte;
    //SDL_Surface* texte2;

    SDL_Rect rect1={0,0,0,0};

    rect1.x=25;
    rect1.y=50;
    ostringstream oss;
    oss << "     Enregistrement";
    texte = TTF_RenderText_Blended(police, oss.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);

    rect1.x=475;
    ostringstream oss1;
    oss1 << "     Audio";
    SDL_FreeSurface(texte);
    texte = TTF_RenderText_Blended(police, oss1.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);

    for(int i=0; (i<6 && (i<capt_devices.size() || i<out_devices.size())); i++)
    {
        rect1.y=118+i*68;
        rect1.x=25;
        SDL_BlitSurface(audio,nullptr,fene_menu,&rect1);

        rect1.x=475;
        SDL_BlitSurface(audio,nullptr,fene_menu,&rect1);

    }

    //bouton defaut
    rect1.y=532;
    rect1.x=225;
    SDL_BlitSurface(standard,nullptr,fene_menu,&rect1);
    ostringstream oss3;
    oss3 << "     Defaut";
    SDL_FreeSurface(texte);
    texte = TTF_RenderText_Blended(police, oss3.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);



    //bouton quitter
    rect1.x=475;
    SDL_BlitSurface(quitter,nullptr,fene_menu,&rect1);
    ostringstream oss4;
    oss4 << "     Quitter";
    SDL_FreeSurface(texte);
    texte = TTF_RenderText_Blended(police, oss4.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);

    SDL_FreeSurface(commencer);
    SDL_FreeSurface(texte);
    //SDL_FreeSurface(texte2);
    SDL_FreeSurface(standard);
    SDL_FreeSurface(quitter);
}

void Menu_option::update()
{
    SDL_BlitSurface(fene_menu,nullptr,screen,nullptr);
    SDL_Rect rect1={25,0,0,0};
    SDL_Color couleur = {0, 0, 0, 42};
    SDL_Surface* texte;

    for(unsigned int i=incr_gch; (i<capt_devices.size() && i<incr_gch+6); i++)
    {
        rect1.y=125+(i-incr_gch)*68;
        ostringstream oss;
        oss << capt_devices[i];
        texte = TTF_RenderText_Blended(police, oss.str().c_str(), couleur);

		SDL_BlitSurface(texte,nullptr,screen,&rect1);
        SDL_FreeSurface(texte);
    }

    rect1.x=475;
    for(unsigned int i=incr_dte; (i<out_devices.size() && i<incr_dte+6); i++)
    {
        rect1.y=125+(i-incr_dte)*68;

        ostringstream oss;
        oss << out_devices[i];
        texte = TTF_RenderText_Blended(police, oss.str().c_str(), couleur);

		SDL_BlitSurface(texte,nullptr,screen,&rect1);
        SDL_FreeSurface(texte);
    }

    SDL_Flip(screen);
}

Menu_option::~Menu_option()
{
	SDL_FreeSurface(fene_menu);
	TTF_CloseFont(police);
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

                            //commencer
                            if(x>=525 && x<525+204
                               && y>=ligne && y<ligne+43
                               && (i+incr)<list_util.size())
                            {
				if(all_enreg(i+incr))
				{
                                	Joueur j(i+incr, list_util[i+incr]);
                               		Game g(screen,&alsc,j);
	                                g.start();
								screen=SDL_SetVideoMode(fene_menu->w,
									fene_menu->h,
									fene_menu->format->BitsPerPixel ,
									fene_menu->flags);
				}
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

Menu_enregistrement::Menu_enregistrement(SDL_Surface* scre, std::vector<std::string> list, int ind)
:screen(SDL_SetVideoMode(800, 600, scre->format->BitsPerPixel, scre->flags
		//	SDL_HWSURFACE | SDL_DOUBLEBUF
		)), fene_menu(SDL_CreateRGBSurface(screen->flags,
		screen->w,
    screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
	screen->format->Amask)),
police(TTF_OpenFont("data/Arial.ttf", 25)),
list_util(list), index(ind), nom_temp("")
{
    if(ind!=-1)
    {
        nom_temp=list_util[index];
    }
    draw();
    update();
}

void Menu_enregistrement::start()
{
    std::chrono::milliseconds dura(40);
    while(!gestion_clic())
    {
        update();
		std::this_thread::sleep_for(dura);
    }
    return;
}

Menu_enregistrement::~Menu_enregistrement()
{

}

void Menu_enregistrement::draw()
{
    SDL_WM_SetCaption("Enregistrement des sons", nullptr);
    SDL_FillRect(fene_menu, nullptr, SDL_MapRGB(fene_menu->format, 17, 206, 112));

    SDL_Surface* commencer = IMG_Load( "../../images/buttun_begin.png" );
    SDL_Surface* standard = IMG_Load( "../../images/buttun.png" );
    SDL_Surface* quitter = IMG_Load( "../../images/buttun_quit.png" );
    SDL_Surface* sond = IMG_Load("../../images/buttun_sound.png");
    nom_sond = {"gauche","droite","haut","bas","tir"};

    SDL_Rect rect1={0,0,0,0};
    SDL_Color couleur = {0, 0, 0, 42};
    SDL_Surface* texte;

    //nom et champ a saisir
    rect1.y=25;
    rect1.x=146;
    SDL_BlitSurface(standard,nullptr,fene_menu,&rect1);
    rect1.x=160;
    if(index!=-1)
    {
        ostringstream oss_nom;
        oss_nom << list_util[index];
        texte=TTF_RenderText_Blended(police, oss_nom.str().c_str(), couleur);
        SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);
            SDL_FreeSurface(texte);
    }

    SDL_Rect rect2 = {350,25,204,43};
    SDL_FillRect(fene_menu, &rect2, SDL_MapRGB(fene_menu->format, 255, 255, 255));

    //boutons des sonds
    vector<string> mots;
    for(int i=0; i<nom_sond.size(); i++)
    {
        rect1.y=100+i*75;
        for(int j=0; j<2; j++)
        {
            rect1.x=25+j*400;
            SDL_BlitSurface(sond,nullptr,fene_menu,&rect1);
            ostringstream oss;
            oss << nom_sond[i] << (j+1);
            texte = TTF_RenderText_Blended(police, oss.str().c_str(), couleur);
            SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);


            rect1.x=150+j*400;
            SDL_BlitSurface(sond,nullptr,fene_menu,&rect1);
            ostringstream oss1;
            rect1.x=160+j*400;
            oss1 << "enreg";
            SDL_FreeSurface(texte);
            texte = TTF_RenderText_Blended(police, oss1.str().c_str(), couleur);
            SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);


            rect1.x=275+j*400;
            SDL_BlitSurface(sond,nullptr,fene_menu,&rect1);
            ostringstream oss2;
            oss2 << "lire";
            rect1.x=285+j*400;
            SDL_FreeSurface(texte);
            texte = TTF_RenderText_Blended(police, oss2.str().c_str(), couleur);
            SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);
            SDL_FreeSurface(texte);

        }
    }

    //bouton quitter
    rect1.x=99;
    rect1.y=525;
    SDL_BlitSurface(quitter,nullptr,fene_menu,&rect1);
    ostringstream oss4;
    oss4 << "     Quitter";
    //SDL_FreeSurface(texte);
    texte = TTF_RenderText_Blended(police, oss4.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);

    //bouton commencer
    rect1.x=499;
    rect1.y=525;
    SDL_BlitSurface(commencer,nullptr,fene_menu,&rect1);
    ostringstream oss5;
    oss5 << "     Commencer";
    SDL_FreeSurface(texte);
    texte = TTF_RenderText_Blended(police, oss5.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);

    SDL_FreeSurface(commencer);
    SDL_FreeSurface(standard);
    SDL_FreeSurface(quitter);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(sond);
}

void Menu_enregistrement::update()
{
    SDL_BlitSurface(fene_menu,nullptr,screen,nullptr);

    SDL_Rect rect2 = {350,25,204,43};
    SDL_FillRect(fene_menu, &rect2, SDL_MapRGB(fene_menu->format, 255, 255, 255));
    SDL_Surface* texte;
    SDL_Color couleur = {0, 0, 0, 42};

    ostringstream oss_nom;
    oss_nom << nom_temp;
    texte=TTF_RenderText_Blended(police, oss_nom.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);

    SDL_FreeSurface(texte);
    SDL_Flip(screen);
}

bool Menu_enregistrement::gestion_clic()
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
                    case SDLK_BACKSPACE:
                        if(nom_temp.size()>0)
                        {
                            nom_temp=nom_temp.substr(0,nom_temp.size()-1);
                        }
                        break;
        			default:
                        if(event.key.keysym.sym>=SDLK_a &&
                           event.key.keysym.sym<=SDLK_z
                           && nom_temp.size()<9)
                        {
                            nom_temp+=event.key.keysym.sym;
                        }
            			break;
                }
            case SDL_MOUSEBUTTONDOWN:
                {
                int x = event.button.x;
                int y = event.button.y;

                cout << "clik" << endl;

                    for(int i=0; i<4; i++)
                    {
                        if(y>=100+i*75 && y<143+i*75)
                        {
                            for(int j=0; j<2; j++)
                            {
                                if(x>=150+j*400 && x<250+j*400)
                                {
                                    cout << "enre " << nom_sond[i] << j << endl;

                                    SDL_Rect rect2 = {50,425,800,43};
                                    SDL_Surface* texte;
                                    SDL_Color couleur = {0, 0, 0, 42};
                                    SDL_FillRect(fene_menu, &rect2, SDL_MapRGB(fene_menu->format, 17, 206, 112));
                                    ostringstream oss_message;
                                    ostringstream oss1;

                                    oss1 << "veillez parler";
                                    texte=TTF_RenderText_Blended(police, oss1.str().c_str(), couleur);
                                    SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                                    SDL_FreeSurface(texte);

                                    update();

                                    vector<ALshort> samples;
									samples = alsc.wait_sound();

                                    //verifier si le son a un debut
                                    if(indice_debut(equalize_spectrogramme(spectrogramme(MyAL::sample_to_double(samples),
                                                alsc.getSampleRate())))!=-1)
                                    {
                                         //sauvegarde
                                        ostringstream oss;
										if(index==-1)
										{
                                        	oss << "./data/" << list_util.size() << "_" << nom_sond[i] << "_" << (j+1) << ".wav";
										}
										else
										{
                                        	oss << "./data/" << index << "_" << nom_sond[i] << "_" << (j+1) << ".wav";
										}
										MyAL::save_sound(oss.str().c_str(), samples);
                                        SDL_FillRect(fene_menu, &rect2, SDL_MapRGB(fene_menu->format, 17, 206, 112));

                                        oss_message << "son sauvegarde";
                                        texte=TTF_RenderText_Blended(police, oss_message.str().c_str(), couleur);
                                        SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                                        SDL_FreeSurface(texte);
                                    }
                                    else
                                    {
                                        SDL_FillRect(fene_menu, &rect2, SDL_MapRGB(fene_menu->format, 17, 206, 112));

                                        oss_message << "pas de debut detecte dans le son, veillez recommencer";
                                        texte=TTF_RenderText_Blended(police, oss_message.str().c_str(), couleur);
                                        SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                                        SDL_FreeSurface(texte);
                                    }
                                }

                                if(x>=275+j*400 && x<375+j*400)
                                {

                                    ostringstream oss;
									if(index == -1)
	                                    oss << "./data/" << list_util.size() << "_" << nom_sond[i] << "_" << (j+1) << ".wav";
									else
	                                    oss << "./data/" << index << "_" << nom_sond[i] << "_" << (j+1) << ".wav";


                                    SDL_Rect rect2 = {50,425,800,43};
                                    SDL_Surface* texte;
                                    SDL_Color couleur = {0, 0, 0, 42};
                                    SDL_FillRect(fene_menu, &rect2, SDL_MapRGB(fene_menu->format, 17, 206, 112));
                                    ostringstream oss_message;



                                    AL_Play alp;
                                    try
                                    {
                                        alp.put_sound_in_buffer(oss.str().c_str());
                                        alp.play();
                                        while(alp.is_playing());
                                        alp.stop_playing();

                                        oss_message << "lecture du son";
                                        texte=TTF_RenderText_Blended(police, oss_message.str().c_str(), couleur);
                                        SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                                        SDL_FreeSurface(texte);
                                    }
                                    catch(exception e)
                                    {

                                        oss_message << "son introuvable";
                                        texte=TTF_RenderText_Blended(police, oss_message.str().c_str(), couleur);
                                        SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                                        SDL_FreeSurface(texte);
                                    }
                                }
                            }
                        }


                    }

                if(x>=99 && x<=(99+204)
                    && y>=525 && y<=(525+43))
                {
                    cout << "Quitter" << endl;
                    return true;
                }

                if(x>=499 && x<=(499+204)
                    && y>=525 && y<=(525+43))
                {
                    cout << "Commencer" << endl;
					if(nom_temp!="")
					{
					if(all_enreg())
					{
						if(index==-1)
						{
							index=list_util.size();
						}
                    ofstream ofs("./data/joueur.lst");
                    for(int i=0; i<index; i++)
                    {
                        ofs << list_util[i] << endl;
                    }
                    ofs << nom_temp << endl;
                    for(int i=index+1; i<list_util.size(); i++)
                    {
                        ofs << list_util[i] << endl;
                    }

					ofs.close();
                    Joueur j(index, nom_temp);
                    Game g(screen,&alsc,j);
                    g.start();
                    screen=SDL_SetVideoMode(fene_menu->w,
                        fene_menu->h,
                        fene_menu->format->BitsPerPixel ,
                        fene_menu->flags);

                    return true;
                }
                        SDL_Rect rect2 = {50,425,800,43};
                        SDL_Surface* texte;
                        SDL_Color couleur = {0, 0, 0, 42};
                        SDL_FillRect(fene_menu, &rect2,
                            SDL_MapRGB(fene_menu->format, 17, 206, 112));

                        ostringstream oss_message;

                        oss_message << "Veillez enregister tous les sons";
                        texte=TTF_RenderText_Blended(police,
                                oss_message.str().c_str(), couleur);
                        SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                        SDL_FreeSurface(texte);
                    }
                    else
                    {
                        SDL_Rect rect2 = {50,425,800,43};
                        SDL_Surface* texte;
                        SDL_Color couleur = {0, 0, 0, 42};
                        SDL_FillRect(fene_menu, &rect2,
                            SDL_MapRGB(fene_menu->format, 17, 206, 112));
                        ostringstream oss_message;

                        oss_message << "Veillez entrer un nom";
                        texte=TTF_RenderText_Blended(police,
                                    oss_message.str().c_str(), couleur);
                        SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                        SDL_FreeSurface(texte);

                    }
                }

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

bool Menu_enregistrement::is_readable( const string & file )
{
    ifstream fichier( file.c_str() );
    if(fichier.fail())
		return false;
	fichier.close();
	return true;
}

bool Menu_enregistrement::all_enreg()
{
    for(int i=0; i<nom_sond.size(); i++)
    {
        for(int j=0; j<2; j++)
        {
            ostringstream oss;
            if(index==-1)
            {
                oss << "./data/" << list_util.size() << "_" << nom_sond[i]
                    << "_" << (j+1) << ".wav";
            }
            else
            {
                oss << "./data/" << index << "_" << nom_sond[i] << "_"
                    << (j+1) << ".wav";
            }
            if(!is_readable(oss.str().c_str()))
            {
                return false;
            }
        }
    }
    return true;
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



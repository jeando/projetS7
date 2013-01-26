#include "Menu_Option.h"
#include"Game.h"
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;
extern AL_Stream_Capture alsc;
//extern AL_Stream_Capture_And_Play alsc;



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

        //traitement des mots trop longs
        for(int j=0; j<3; j++)
        {
            getline(iss,tmp, ' ');
            if(tmp=="")
            {
                break;
            }
            nom+=" ";
            nom+=tmp;
        }

        for(int j=1; j<nom.size(); j++)
        {
            int ascii = ((int)nom[j]);
            cout << " " << nom[j] << ascii;
            if(ascii==-87)//"é"
            {
                nom[j]='e';
            }
            if(ascii==-61)
            {
                nom.replace(j,1,"");
                j--;
            }
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
        for(int j=1; j<nom.size(); j++)
        {
            int ascii = ((int)nom[j]);
            cout << " " << nom[j] << ascii;
            if(ascii==-87)//"é"
            {
                nom[j]='e';
            }
            if(ascii==-61)
            {
                nom.replace(j,1,"");
                j--;
            }
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

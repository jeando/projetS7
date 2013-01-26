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
    SDL_Surface* sond2 = IMG_Load("../../images/buttun_sound2.png");
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
            ostringstream ossR;
            ossR << "./data/" << index << "_" << nom_sond[i] << "_"
                 << (j+1) << ".wav";

            if(is_readable(ossR.str().c_str()))
            {
                SDL_BlitSurface(sond,nullptr,fene_menu,&rect1);
            }
            else
            {
                SDL_BlitSurface(sond2,nullptr,fene_menu,&rect1);
            }
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

                    for(int i=0; i<nom_sond.size(); i++)
                    {
                        if(y>=100+i*75 && y<143+i*75)
                        {
                            for(int j=0; j<2; j++)
                            {
                                if(x>=150+j*400 && x<250+j*400)
                                {
                                    cout << "enre " << nom_sond[i] << j << endl;

                                    SDL_Rect rect2 = {50,475,800,43};
                                    SDL_Surface* texte;
                                    SDL_Color couleur = {0, 0, 0, 42};
                                    SDL_FillRect(fene_menu, &rect2, SDL_MapRGB(fene_menu->format, 17, 206, 112));
                                    ostringstream oss_message;
                                    ostringstream oss1;

                                    oss1 << "veuillez parler";
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
                                        //SDL_FreeSurface(texte);

                                        AL_Play alp;
                                        alp.put_sound_in_buffer(oss.str().c_str());
                                        alp.play();
                                        while(alp.is_playing());
                                        alp.stop_playing();


                                        SDL_Surface* sond = IMG_Load("../../images/buttun_sound.png");
                                        SDL_Rect rect1 = {0,0,100,43};
                                        SDL_Rect rect2 = {
                                            static_cast<Sint16>(275+j*400),
                                            static_cast<Sint16>(100+i*75),100,43};
                                        SDL_BlitSurface(sond,&rect1,fene_menu,&rect2);
                                        SDL_UpdateRect(screen, rect2.x, rect2.y, rect2.w, rect2.h);
                                        ostringstream oss2;
                                        oss2 << "lire";
                                        rect2.x=285+j*400;
                                        SDL_FreeSurface(texte);
                                        texte = TTF_RenderText_Blended(police, oss2.str().c_str(), couleur);
                                        SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);
                                        SDL_FreeSurface(texte);
                                        SDL_FreeSurface(sond);

                                    }
                                    else
                                    {
                                        SDL_FillRect(fene_menu, &rect2, SDL_MapRGB(fene_menu->format, 17, 206, 112));

                                        oss_message << "pas de debut detecte dans le son, veuillez recommencer";
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


                                    SDL_Rect rect2 = {50,475,800,43};
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
                        else
                        {
                            SDL_Rect rect2 = {50,475,800,43};
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
                        SDL_Rect rect2 = {50,475,800,43};
                        SDL_Surface* texte;
                        SDL_Color couleur = {0, 0, 0, 42};
                        SDL_FillRect(fene_menu, &rect2,
                            SDL_MapRGB(fene_menu->format, 17, 206, 112));
                        ostringstream oss_message;

                        oss_message << "Veuillez entrer un nom";
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

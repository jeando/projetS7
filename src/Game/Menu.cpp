#include "Menu.h"
#include"Game.h"
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <sstream>
using namespace std;

Menu::Menu(SDL_Surface* scre)
:alsc(), game(scre, &alsc), screen(scre), fene_menu(SDL_CreateRGBSurface(scre->flags, scre->w,
    scre->h, scre->format->BitsPerPixel, scre->format->Rmask, scre->format->Gmask, scre->format->Bmask, scre->format->Amask))
{
    draw();
    update();
}

void Menu::draw()
{
    SDL_WM_SetCaption("Menu", nullptr);
    TTF_Init();
    SDL_FillRect(fene_menu, nullptr, SDL_MapRGB(fene_menu->format, 17, 206, 112));

    SDL_Surface* commencer = IMG_Load( "../../images/buttun_begin.png" );
    SDL_Surface* standard = IMG_Load( "../../images/buttun.png" );
    SDL_Surface* quitter = IMG_Load( "../../images/buttun_quit.png" );

    TTF_Font* police = TTF_OpenFont("./data/Arial.ttf", 35);
    SDL_Color couleur = {0, 0, 0, 42};

    //positionnement et affichage des boutons
    /*rect1.x=620;
    rect1.y=20;
    SDL_BlitSurface(commencer,nullptr,fene_menu,&rect1);
    ostringstream oss;
    oss << " Commencer";
    SDL_Surface* texte = TTF_RenderText_Blended(police, oss.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect1);*/

    rect2.x=400;
    rect2.y=350;
    SDL_BlitSurface(standard,nullptr,fene_menu,&rect2);
    ostringstream oss2;
    oss2 << "   Utilisateur";
    SDL_Surface* texte = TTF_RenderText_Blended(police, oss2.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect2);

    rect3.x=900;
    rect3.y=350;
    SDL_BlitSurface(standard,nullptr,fene_menu,&rect3);
    ostringstream oss3;
    oss3 << "    Options";
    texte = TTF_RenderText_Blended(police, oss3.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect3);

    rect4.x=620;
    rect4.y=700;
    SDL_BlitSurface(quitter,nullptr,fene_menu,&rect4);
    ostringstream oss4;
    oss4 << "     Quitter";
    texte = TTF_RenderText_Blended(police, oss4.str().c_str(), couleur);
    SDL_BlitSurface(texte,nullptr,fene_menu,&rect4);

    SDL_FreeSurface(commencer);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(standard);
    SDL_FreeSurface(quitter);
    TTF_Quit();
}

void Menu::update()
{
    SDL_BlitSurface(fene_menu,nullptr,screen,nullptr);
    SDL_Flip(screen);
}

void Menu::start()
{
    std::chrono::milliseconds dura(200);
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
                        Game g(screen,&alsc);
                        g.start();
                }*/
                if(x>=rect2.x && x<=(rect2.x+204)
                   && y>=rect2.y && y<=(rect2.y+43))
                {
                        cout << "Choix utilisateur" << endl;
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

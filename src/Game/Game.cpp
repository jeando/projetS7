#include "Game.h"
#include "Menu.h"
using namespace std;

Game::Game(SDL_Surface* scre)
:screen(scre), map("map1",scre)
{
        SDL_WM_SetCaption("Word recognition", NULL);

        map.draw(screen);
}

Game::~Game()
{

}

void Game::pause()
{
    cout << "pause!" << endl;
}

bool Game::gestionSDL_Event()
{

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_p:
                        pause();
                        break;
                    case SDLK_ESCAPE:
                        return false;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                return false;
                break;
            default:
                break;
        }
    }
    return true;
}

bool Game::gestion_Audio()
{
    events_audio test = SON_BAS;//a changer avec la fonction adequoite
    bool deplacement = true;
    switch(test)
    {
        case SON_HAUT:
            deplacement = map.change_speed(0,-1);
            break;
        case SON_BAS:
            deplacement = map.change_speed(0,1);
            break;
        case SON_GAUCHE:
            deplacement = map.change_speed(-1,0);
            break;
        case SON_DROITE:
            deplacement = map.change_speed(1,0);
            break;
        default:
            break;
    }
    return deplacement;
}


void Game::start()
{
    while(gestionSDL_Event())
    {
        if(!gestion_Audio())
        {
            cout << "deplacement impossible" << endl;
        }
        map.update(screen);
        system("sleep 0.3");
    }
    return;
}

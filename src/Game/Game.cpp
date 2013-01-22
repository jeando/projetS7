#include "Game.h"
#include "Menu.h"
using namespace std;

Game::Game(SDL_Surface* scre, AL_Stream_Capture* _alsc, Joueur& _joueur)
:screen(SDL_SetVideoMode(1441, 751, 32, SDL_HWSURFACE|SDL_DOUBLEBUF)), alsc(_alsc), joueur(&_joueur), map("../../images/lab_testjp3.png",screen, _alsc)
{
        SDL_WM_SetCaption("Word recognition", NULL);
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
                    case SDLK_z:
                        map.change_speed(0,-1);
                        break;
                    case SDLK_UP:
                        map.change_speed(0,-1);
                        break;
                    case SDLK_q:
                        map.change_speed(-1,0);
                        break;
                    case SDLK_LEFT:
                        map.change_speed(-1,0);
                        break;
                    case SDLK_s:
                        map.change_speed(0,1);
                        break;
                    case SDLK_DOWN:
                        map.change_speed(0,1);
                        break;
                    case SDLK_d:
                        map.change_speed(1,0);
                        break;
                    case SDLK_RIGHT:
                        map.change_speed(1,0);
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
//    events_audio test = alsc->poll_event();
    events_audio test = alsc->poll_event_continue();
//    bool deplacement = true;
    switch(test)
    {
        case SON_HAUT:
  //          deplacement = map.change_speed(0,-1);
            map.change_speed(0,-1);
            break;
        case SON_BAS:
    //        deplacement = map.change_speed(0,1);
            map.change_speed(0,1);
            break;
        case SON_GAUCHE:
      //      deplacement = map.change_speed(-1,0);
            map.change_speed(-1,0);
            break;
        case SON_DROITE:
        //    deplacement = map.change_speed(1,0);
            map.change_speed(1,0);
            break;
        case SON_TIR:
            map.tir();
            break;
        case RIEN:
            break;
        default:
            break;
    }
    return true;
}


void Game::start()
{
	//std::cout << __FILE__ << " " << __LINE__ << "remplacer le system sleep 0.3" << std::endl;
    map.draw();
    map.update();
	alsc->set_joueur(joueur);
    std::chrono::milliseconds dura(80);
    while(gestionSDL_Event()&&gestion_Audio())
    {
        /*if(!gestion_Audio())
        {
            cout << "deplacement impossible" << endl;
        }*/
        map.update();
	if(map.victoire())
	{
		cout<<"victoire !"<<endl;
		
		SDL_Rect rect2 = {350,25,204,43};
    		SDL_FillRect(screen, &rect2, SDL_MapRGB(screen->format, 255, 255, 255));
		SDL_Surface* texte;
    		SDL_Color couleur = {0, 0, 0, 42};
		SDL_BlitSurface(texte,nullptr,screen,&rect2);
		TTF_Font* police(TTF_OpenFont("data/Arial.ttf", 25));
    		texte=TTF_RenderText_Blended(police,("Victoire "+joueur->nom_joueur+ " !!!").c_str(), couleur);
   		SDL_BlitSurface(texte,nullptr,screen,&rect2);
		SDL_FreeSurface(texte);
		TTF_CloseFont(police);
		//SDL_UpdateRect(screen,rect2.x,rect2.y,rect2.w,rect2.h);
		SDL_UpdateRects(screen,1,&rect2);
		std::this_thread::sleep_for(dura*15);
		break;
	}
		std::this_thread::sleep_for(dura);
    }
	alsc->stop_stream_capture();
    return;
}

#include "Map.h"

#include <SDL/SDL_image.h>
using namespace std;

Map::Map(string nom, SDL_Surface* screen)
:surface(SDL_CreateRGBSurface(screen->flags, screen->w,
    screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask)),
    size_box_x(37), size_box_y(24),
w_map(41),croa_croa(Frog("frog"))
{
    load_map(nom);
}

Map::~Map()
{
    for(Item* ptr : list_items)
    {
        delete ptr;
    }

    for(map<string, SDL_Surface*>::iterator it = surfaces_map.begin(); it!=surfaces_map.end(); it++)
    {
        SDL_FreeSurface(it->second);
    }
}

void Map::load_map(string nom)
{
    list_items.resize(w_map*w_map);
    cout << nom << endl;
    surfaces_map["wall"]=IMG_Load( "../../images/wall.png" );
    surfaces_map["end"]=IMG_Load( "../../images/end.png" );
    surfaces_map["begin"]=IMG_Load( "../../images/begin.png" );
    surfaces_map["background"]=IMG_Load( "../../images/background.png" );
    surfaces_map["frog"]=IMG_Load( "../../images/frog.png" );

    list_items[5] = new Item("wall");
    list_items[19] = new Item("wall");
    list_items[1079] = new Item("end");
    list_items[0] = new Item("begin");
}

void Map::draw(SDL_Surface* screen)
{
    int cpt=0;
    for(Item* ptr : list_items)
    {
        SDL_Rect rect;
        rect.x=size_box_x*(cpt%w_map);
        rect.y=size_box_y*(cpt/w_map);
        SDL_BlitSurface(surfaces_map["background"],nullptr,surface,&rect);
        cpt++;
        if(ptr!=nullptr)
            SDL_BlitSurface(surfaces_map[ptr->nom_image],nullptr,surface,&rect);
    }
        SDL_BlitSurface(surface,nullptr,screen,nullptr);


        SDL_Rect rect1;
        rect1.x=size_box_x*(croa_croa.etat_x);
        rect1.y=size_box_y*(croa_croa.etat_y);
        rect1.w=size_box_x;
        rect1.h=size_box_y;
        SDL_Rect rect2;
        rect2.x=size_box_x*croa_croa.position_x;
        rect2.y=size_box_y*croa_croa.position_y;
        SDL_BlitSurface(surfaces_map["frog"],&rect1,screen,&rect2);
        SDL_Flip(screen);
}

void Map::update(SDL_Surface* screen)//, unsigned int x, unsigned int y);
{
        SDL_Rect rect;
        rect.x=size_box_x*croa_croa.position_x;
        rect.y=size_box_y*croa_croa.position_y;
        rect.w=size_box_x;
        rect.h=size_box_y;
        SDL_BlitSurface(surface,&rect,screen,&rect);

        croa_croa.position_x+=croa_croa.vitesse_x;
        croa_croa.position_y+=croa_croa.vitesse_y;


        croa_croa.etat_x += abs(croa_croa.vitesse_x) + abs(croa_croa.vitesse_y);
        while(croa_croa.etat_x>3)
        {
            croa_croa.etat_x-=4;
        }

        SDL_Rect rect1;
        rect1.x=size_box_x*(croa_croa.etat_x);
        rect1.y=size_box_y*(croa_croa.etat_y);
        rect1.w=size_box_x;
        rect1.h=size_box_y;

        SDL_Rect rect2;
        rect2.x=size_box_x*croa_croa.position_x;
        rect2.y=size_box_y*croa_croa.position_y;
        SDL_BlitSurface(surfaces_map["frog"],&rect1,screen,&rect2);
        SDL_Flip(screen);
}

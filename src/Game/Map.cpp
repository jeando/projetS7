#include "Map.h"

#include <SDL/SDL_image.h>
using namespace std;

Map::Map(string nom, SDL_Surface* screen)
:surface(SDL_CreateRGBSurface(screen->flags, screen->w,
    screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask)),
size_box_x(30), size_box_y(30), w_map(48), h_map(25),croa_croa(Frog("frog"))
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
    cout << nom << endl;

    surfaces_map["wall"]=IMG_Load( "../../images/wall.png" );
    surfaces_map["end"]=IMG_Load( "../../images/end.png" );
    surfaces_map["begin"]=IMG_Load( "../../images/begin.png" );
    surfaces_map["background"]=IMG_Load( "../../images/background.png" );
    surfaces_map["frog"]=IMG_Load( "../../images/frog.png" );

    //chargement de l'image a traiter
    const char* img = nom.c_str();
    SDL_Surface* fond = IMG_Load(img);

    //redimentionnement de la map
    w_map = fond->w;
    h_map = fond->h;
    list_items.resize(w_map*h_map);

    SDL_LockSurface(fond); // Vérouillage de la surface
    Uint8* pPixels = ((Uint8*)fond->pixels);
    Uint8 red, green, blue; // Variables servant à stocker la couleur du pixel choisi
    for(int i=0; i<w_map; i++)
    {
        for(int j=0; j<h_map; j++)
        {
            //cout << i << " " << j << " ";
            //SDL_GetRGB(j*w_map+(i+1), fond->format, &red, &green, &blue);
            red = *(pPixels+4*(j*w_map+(i+1)));
            green = *(pPixels+4*(j*w_map+(i+1)+1));
            blue = *(pPixels+4*(j*w_map+(i+1)+2));

            int r = ((int)red);
            int g = ((int)green);
            int b = ((int)blue);
            cout << ((int)r) << " " << ((int)g) << " " << ((int)b) << "|| ";

            //Si le fond etait rouge
            if( red==0xFF && green==0x00 && blue==0x00)//red==255 && green==0 && blue==0)//==>meme resultat
            {
                list_items[i+j*w_map] = new Item("end");


            }
            //Si le fond etait vert
            if(  red==0x00 && green==0xFF && blue==0x00)//green == 255 && blue==0 && red==0)
            {
                list_items[w_map*j+i] = new Item("wall");
            }
            //Si le fond etait bleu
            if( red==0x00 && green==0x00 && blue==0xFF)//blue==255 && green==0 && red==0)
            {
                int debut_x=i;
                int debut_y=j;
                list_items[debut_y*w_map+debut_x] = new Item("begin");
                croa_croa.position_x=debut_x;
                croa_croa.position_y=debut_y;
            }
        }
        cout << endl;
    }

    SDL_UnlockSurface(fond); //Dévérouillage de la surface

    SDL_FreeSurface(fond);

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
    if(is_deplacement_possible(croa_croa.vitesse_x, croa_croa.vitesse_y))
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

}

bool Map::is_deplacement_possible(int vx, int vy)
{
    int coord = (croa_croa.position_x+vx)+w_map*(croa_croa.position_y+vy);
    if((coord<list_items.size() //limite basse
        && croa_croa.position_x+vx<w_map//limite droite
        && coord>0 //limite haute
        && croa_croa.position_x+vx>=0 //limite gauche
        )
       && ((list_items[coord]!=nullptr && list_items[coord]->nom_image!="wall")
            || list_items[coord]==nullptr))//gestion des elements presents sur le terrain
    {
        return true;
    }
    return false;
}

bool Map::change_speed(int vx, int vy)
{
    if(is_deplacement_possible(vx, vy))
    {
        if(vx>0)
        {
            croa_croa.etat_x=0;
            croa_croa.etat_y=3;
        }
        else if(vx==0)
        {
            if(vy>0)
            {
                croa_croa.etat_x=0;
                croa_croa.etat_y=5;
            }
            else if(vy==0)
            {
                if(croa_croa.vitesse_x>=0)
                {
                    croa_croa.etat_x=0;
                    croa_croa.etat_y=2;
                }
                else
                {
                    croa_croa.etat_x=0;
                    croa_croa.etat_y=0;
                }
            }
            else
            {
                croa_croa.etat_x=0;
                croa_croa.etat_y=4;
            }

        }
        else
        {
            croa_croa.etat_x=0;
            croa_croa.etat_y=1;
        }
        croa_croa.vitesse_x = vx;
        croa_croa.vitesse_y = vy;
        return true;
    }
    return false;
}

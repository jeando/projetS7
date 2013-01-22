#include "Map.h"

#include <SDL/SDL_image.h>
using namespace std;

Map::Map(string nom, SDL_Surface* _screen, AL_Stream_Capture* _alsc)
:screen(_screen),surface(SDL_CreateRGBSurface(screen->flags, screen->w,
    screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask,
	screen->format->Amask)),alsc(_alsc),
size_box_x(30), size_box_y(30), w_map(48), h_map(25),croa_croa(Frog("frog")),cpt_position(0)
{

    surfaces_map["wall"]=IMG_Load( "../../images/wall.png" );
    surfaces_map["end"]=IMG_Load( "../../images/end.png" );
    surfaces_map["begin"]=IMG_Load( "../../images/begin.png" );
    surfaces_map["background"]=IMG_Load( "../../images/background.png" );
    surfaces_map["frog"]=IMG_Load( "../../images/frog.png" );
    surfaces_map["tree"]=IMG_Load( "../../images/tree.png" );

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
	SDL_FreeSurface(surface);
}

void Map::load_map(string nom)
{
    cout << nom << endl;


    //chargement de l'image a traiter
    const char* img = nom.c_str();
    SDL_Surface* fond = IMG_Load(img);

    //redimentionnement de la map
    w_map = fond->w;
    h_map = fond->h;
    list_items.resize(w_map*h_map);

    Uint8* pPixels = nullptr;
    int  red, green, blue; // Variables servant à stocker la couleur du pixel choisi
    int bpp = fond->format->BytesPerPixel;
    for(int i=0; i<w_map; i++)
    {
        for(int j=0; j<h_map; j++)
        {
            pPixels = ((Uint8*)fond->pixels)+j*fond->pitch+i*bpp;
            switch(bpp)
            {
                case 3:
                    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    {
                        red = pPixels[0] << 16;
                        green = pPixels[1] << 8;
                        blue = pPixels[2];
                    }
                    else
                    {
                        red = pPixels[0];
                        green = pPixels[1] << 8;
                        blue = pPixels[2] << 16;
                    }
                    break;
                case 4:
                    red = pPixels[0] << 24;
                    green = pPixels[1] << 16;
                    blue = pPixels[2] << 8;
            }

            //Si le fond etait rouge
            if( red==fond->format->Rmask && green==0x00 && blue==0x00)
            {
                list_items[i+j*w_map] = new Item("end");
            }
            //Si le fond etait vert
            if(  red==0x00 && green==fond->format->Gmask && blue==0x00)
            {
                list_items[w_map*j+i] = new Item("wall");
            }
            //Si le fond etait jaune
            if(  red==fond->format->Rmask && green==fond->format->Gmask && blue==0x00)
            {
                list_items[w_map*j+i] = new Item("tree");
            }

            //Si le fond etait bleu
            if( red==0x00 && green==0x00 && blue==fond->format->Bmask)
            {
                int debut_x=i;
                int debut_y=j;
                list_items[debut_y*w_map+debut_x] = new Item("begin");
                croa_croa.position_x=debut_x;
                croa_croa.position_y=debut_y;
            }
        }
    }
    SDL_FreeSurface(fond);
}

void Map::draw()
{
    int cpt=0;
    SDL_Rect rect = {0,0,0,0};
    for(Item* ptr : list_items)
    {
        rect.x=size_box_x*(cpt%w_map);
        rect.y=size_box_y*(cpt/w_map);
        SDL_BlitSurface(surfaces_map["background"],nullptr,surface,&rect);
        cpt++;
        if(ptr!=nullptr)
            SDL_BlitSurface(surfaces_map[ptr->nom_image],nullptr,surface,&rect);
    }
    SDL_BlitSurface(surface,nullptr,screen,nullptr);

   // SDL_Rect rect1;
    rect.x=size_box_x*(croa_croa.etat_x);
    rect.y=size_box_y*(croa_croa.etat_y);
    rect.w=size_box_x;
    rect.h=size_box_y;
    SDL_Rect rect2;
    rect2.x=size_box_x*croa_croa.position_x;
    rect2.y=size_box_y*croa_croa.position_y;
    SDL_BlitSurface(surfaces_map["frog"],&rect,screen,&rect2);
    SDL_Flip(screen);
}

void Map::update()//, unsigned int x, unsigned int y);
{
    if(is_deplacement_possible(croa_croa.vitesse_x, croa_croa.vitesse_y))
    {
        SDL_Rect rect={
        	static_cast<Sint16>(size_box_x*croa_croa.position_x),
        	static_cast<Sint16>(size_box_y*croa_croa.position_y),
        	static_cast<Uint16>(size_box_x),
        	static_cast<Uint16>(size_box_y)
		};
        SDL_BlitSurface(surface,&rect,screen,&rect);

        croa_croa.position_x+=croa_croa.vitesse_x;
        croa_croa.position_y+=croa_croa.vitesse_y;


        croa_croa.etat_x += abs(croa_croa.vitesse_x) + abs(croa_croa.vitesse_y);
		cpt_position++;

		if(croa_croa.vitesse_x==0 && croa_croa.vitesse_y==0){ // clignotement
			croa_croa.etat_x++;
		}

        while(croa_croa.etat_x>3)
        {
            croa_croa.etat_x-=4;
        }

        SDL_Rect rect1{
        	static_cast<Sint16>(size_box_x*croa_croa.etat_x),
        	static_cast<Sint16>(size_box_y*croa_croa.etat_y),
        	static_cast<Uint16>(size_box_x),
        	static_cast<Uint16>(size_box_y)
		};
//*/
        SDL_Rect rect2 = {
        	static_cast<Sint16>(size_box_x*croa_croa.position_x),
        	static_cast<Sint16>(size_box_y*croa_croa.position_y),
			0,
			0
		};
        SDL_BlitSurface(surfaces_map["frog"],&rect1,screen,&rect2);
//        SDL_Flip(screen);
		SDL_UpdateRect(screen, min(rect.x, rect2.x),min(rect.y, rect2.y),
				size_box_x*(rect.x==rect2.x?1:2),
				size_box_y*(rect.y==rect2.y?1:2));
        if(cpt_position==4){
            cpt_position^=cpt_position; // cpt_position=0 mais avec une étape en moins
	//		std::cout << cpt_position << std::endl;
            //cpt_position=0;
			if(croa_croa.vitesse_x>0){//remet une position standard apres chaque deplacement
				croa_croa.etat_y=2;
        		croa_croa.vitesse_x=0;
        		croa_croa.vitesse_y=0;
			}
			else if(croa_croa.vitesse_x<0){
				croa_croa.etat_y=0;
        		croa_croa.vitesse_x=0;
        		croa_croa.vitesse_y=0;
			}
			else if(croa_croa.vitesse_y!=0)
			{
				croa_croa.etat_y=0;
        		croa_croa.vitesse_y=0;
			}
			croa_croa.etat_x=0;
	/*if(croa_croa.vitesse_x>0){//remet une position standard apres chaque deplacement
		croa_croa.etat_y=0;
	}
	if(croa_croa.vitesse_x<0){
		croa_croa.etat_y=2;
	}//*/
			//alsc->start_stream_capture();
		}
    }
}
bool Map::victoire()
{
	coord = (croa_croa.position_x)+w_map*(croa_croa.position_y);
	return croa_croa.etat_x==0 && list_items[coord]!=nullptr && list_items[coord]->nom_image=="end";

}
bool Map::is_deplacement_possible(int vx, int vy)
{
    int coord = (croa_croa.position_x+vx)+w_map*(croa_croa.position_y+vy);
    if((coord<list_items.size() //limite basse
        && croa_croa.position_x+vx<w_map//limite droite
        && coord>0 //limite haute
        && croa_croa.position_x+vx>=0 //limite gauche
        )
       && ((list_items[coord]!=nullptr && list_items[coord]->nom_image!="wall" && list_items[coord]->nom_image!="tree")
            || list_items[coord]==nullptr))//gestion des elements presents sur le terrain
    {
        return true;
    }

    croa_croa.etat_y=8;
    croa_croa.etat_x++;
    while(croa_croa.etat_x>3)
    {
        croa_croa.etat_x-=4;
    }
    return false;
}

void Map::tir()
{
    int coord = (croa_croa.position_x+1)+croa_croa.position_y*w_map;
    SDL_Rect rect;
    rect.w=static_cast<Sint16>(size_box_x);
    rect.h=static_cast<Sint16>(size_box_y);

    if(list_items[coord]!=nullptr && list_items[coord]->nom_image=="tree")
    {
        list_items[coord]=nullptr;
        rect.x = static_cast<Sint16>((croa_croa.position_x+1)*size_box_x);
        rect.y = static_cast<Sint16>((croa_croa.position_y)*size_box_y);
    }
    coord=croa_croa.position_x+(croa_croa.position_y-1)*w_map;
    if(list_items[coord]!=nullptr && list_items[coord]->nom_image=="tree")
    {
        list_items[coord]=nullptr;
        rect.x = static_cast<Sint16>((croa_croa.position_x)*size_box_x);
        rect.y = static_cast<Sint16>((croa_croa.position_y-1)*size_box_y);
    }
    coord=(croa_croa.position_x-1)+croa_croa.position_y*w_map;
    if(list_items[coord]!=nullptr && list_items[coord]->nom_image=="tree")
    {
        list_items[coord]=nullptr;
        rect.x = static_cast<Sint16>((croa_croa.position_x-1)*size_box_x);
        rect.y = static_cast<Sint16>((croa_croa.position_y)*size_box_y);
    }
    coord=croa_croa.position_x+(croa_croa.position_y+1)*w_map;
    if(list_items[coord]!=nullptr && list_items[coord]->nom_image=="tree")
    {
        list_items[coord]=nullptr;
        rect.x = static_cast<Sint16>((croa_croa.position_x)*size_box_x);
        rect.y = static_cast<Sint16>((croa_croa.position_y+1)*size_box_y);
    }

    SDL_Rect rect2 = {30,0,30,30};
    chrono::milliseconds dura(200);
    SDL_BlitSurface(surfaces_map["tree"],&rect2,surface,&rect);
    SDL_BlitSurface(surface,&rect,screen,&rect);
    SDL_UpdateRect(screen, rect.x, rect.y, rect.w, rect.h);
    cout << "test" << endl;
    this_thread::sleep_for(dura);

    rect2.x=60;
    SDL_BlitSurface(surfaces_map["tree"],&rect2,surface,&rect);
    SDL_BlitSurface(surface,&rect,screen,&rect);
    SDL_UpdateRect(screen, rect.x, rect.y, rect.w, rect.h);
    this_thread::sleep_for(dura);

    rect2.x=90;
    SDL_BlitSurface(surfaces_map["tree"],&rect2,surface,&rect);
    SDL_BlitSurface(surface,&rect,screen,&rect);
    SDL_UpdateRect(screen, rect.x, rect.y, rect.w, rect.h);
    this_thread::sleep_for(dura);

    SDL_BlitSurface(surfaces_map["background"],nullptr,surface,&rect);
    SDL_BlitSurface(surface,&rect,screen,&rect);
    SDL_UpdateRect(screen, rect.x, rect.y, rect.w, rect.h);
}

bool Map::change_speed(int vx, int vy)
{
    cpt_position=0;
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

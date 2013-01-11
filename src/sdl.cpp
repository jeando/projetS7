#include"sdl.h"
#include"util_tmp.h"
#include<algorithm>
//assert than val_pixel \in [0, 255]
inline unsigned int get_couleur(double val_pix)
{
	if(val_pix<255/3.0){
		return 0xFF<<16|((Uint8)((0xFF-val_pix)*3.0/2))<<8|0xFF;
	}
	else if(val_pix<255*2.0/3){
		return ((Uint8)(val_pix*3 - 0xFF))<<24|0xFF<<16|((Uint8)(2*0xFF-3*val_pix))<<8|0xFF;
	}
	else{
		return 0xFF<<24|((Uint8)((0xFF-val_pix)*3.0))<<16|0xFF;
	}
}
template<typename T>
void draw_vect(SDL_Surface* screen, std::vector<T> vect)
{
//#include<limits>
//T min = (1<<((sizeof(T)<<3)-1))-1);
	//T min = std::numeric_limits<T>::max();
//	T max = std::numeric_limits<T>::min();
	if(screen->w!=vect.size())
		screen = SDL_SetVideoMode(vect.size(),screen->h,screen->format->BitsPerPixel,screen->flags);
	SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format,0,0,0));
	T max = * std::max_element(vect.begin(),vect.end());
	unsigned int pos(0);
	for(typename std::vector<T>::iterator it= vect.begin();it!=vect.end();it++)
	{
		vlineColor(screen, pos, screen->h, screen->h*(1-(*it)*1.0/max),get_couleur(255* (*it)/max));
		pos++;
	}
	SDL_Flip(screen);
}

template<typename T>
void draw_mat(SDL_Surface* screen, std::vector<std::vector<T> > vect)
{
	
	screen = SDL_SetVideoMode(vect.size(),vect[0].size(),screen->format->BitsPerPixel,screen->flags);
	SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format,0,0,0));
	/*T max(*std::max_element(vect[0].begin(),vect[0].end()));
	T max_tmp;
	for(typename std::vector<std::vector<T> >::iterator it= vect.begin()+1;it!=vect.end();it++)
	{
		max_tmp = *std::max_element(it->begin(),it->end());
		if(max_tmp>max)
			max=max_tmp;
	}*/
	T max(get_max_val(vect));
	unsigned int pos_x(0), pos_y(0);
//	for(unsigned int x(0), x< 
	for(typename std::vector<std::vector<T> >::iterator it= vect.begin();it!=vect.end();it++)
	{
		for(typename std::vector<T>::iterator it_2= it->begin();it_2!=it->end();it_2++){
			pixelColor(screen, pos_x, pos_y ,get_couleur(255* (*it_2)/max));
			pos_y++;
		}
		pos_x++;
		pos_y=0;
//		std::cout << pos_x << " " << pos_y << std::endl;
	}
	SDL_Flip(screen);
}

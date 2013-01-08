#include"MyAL.h"
#include"traitement.h"
#include<iostream>
#include<SDL/SDL.h>
#include<SDL/SDL_gfxPrimitives.h>
int main()
{
   // MyAL al;
    std::vector<std::string> vs = MyAL::get_devices();
    std::vector<std::string> vs2 = MyAL::get_capture_devices();
   int audio_choix = 0;
   int capture_choix = 0;
   int k=0;

   //choix haut-parleurs
   std::cout << "devices :" << std::endl;
   for(std::string s : vs)
        std::cout << "\t" << k++ << ") "<< s << std::endl;
    if(vs.size()>1){
        std::cout << "choix : ";
        std::cin >> audio_choix;
    }
    k=0;

    //choix micro (si 1 seul choix => pas de choix)
   std::cout << "capture devices :" << std::endl;
   for(std::string s : vs2)
        std::cout << "\t" << k++ << ") "<< s << std::endl;
    if(vs2.size()>1){
        std::cout << "choix : ";
        std::cin >> capture_choix;
    }

    //enregistrement:
    std::chrono::milliseconds dura(4000);
    std::vector<ALshort> samples;
    //enregistre
    AL_Capture alc(vs[audio_choix],vs2[capture_choix]);//port audio / micro
    alc.start(samples);
    //dors dura milliseconds
    std::this_thread::sleep_for(dura);
    alc.stop();//arret enregistrement
    std::cout << samples.size() << std::endl;
    //on enregistre le son
    alc.save_sound("test.wav");
	std::vector<double> samples_double;
	for(unsigned int i=0; i<samples.size();i++){
		//modifier convertir en double
		//samples_double.push_back(samples[i]);
		samples_double.push_back(samples[i]/MAX_ALSHORT_VAL);
	}
	std::vector<double> dec = decoupage(samples_double,783,30*alc.getSampleRate()/1000);
	std::cout << dec.size() << std::endl;
	std::vector<std::vector<double> > spectro
		= spectrogramme(samples_double,alc.getSampleRate());
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen(nullptr);
	screen = SDL_SetVideoMode(spectro.size(),256,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));
	double min( (1<<((sizeof(double)<<3)-1))-1);
	double max( (-(1<<((sizeof(ALshort)<<3)-1))));
	for(unsigned int x=0; x<spectro.size(); x++)
	{
		for(unsigned int y=0; y<spectro[x].size(); y++){
			if(spectro[x][y]>max)max=spectro[x][y];
			if(spectro[x][y]<min)min=spectro[x][y];
		}
	}
	std::cout << "min: "<< min << " max: " << max << std::endl;
	for(unsigned int x=0; x<spectro.size(); x++)
	{
		for(unsigned int y=0; y<spectro[x].size(); y++)
		{
			double color_r = 255*(spectro[x][y]+fabs(min))/max;
			if(color_r<0)color_r=0;
			double color_g = 255*((spectro[x][y]+fabs(min))/max+1);
			if(color_g<0)color_g=0;
			if(color_g>255)
				color_g=0;
			double color_b = 255*((spectro[x][y]+fabs(min))/max+2);
			if(color_b<0)color_b=0;
			if(color_b>255)
				color_b=0;
//			pixelColor(screen, x, y, ((int)(color))<<16|0xFF);
			pixelColor(screen, x, y, ((Uint8)color_r)<<24|((Uint8)color_g)<<16|((Uint8)color_b)<<8|0xFF);
		//	pixelColor(screen, x, y, ((int)(spectro[x][y]))<<16|0xFF);
		}
		SDL_Flip(screen);
	}
	SDL_Flip(screen);
	SDL_Flip(screen);
    std::this_thread::sleep_for(dura);
	std::cout << max << std::endl;
}

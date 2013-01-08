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
	double max(0);
	for(unsigned int x=0; x<spectro.size(); x++)
	{
		for(unsigned int y=0; y<spectro[x].size(); y++){
			if(spectro[x][y]>max)max=spectro[x][y];
		}
	}
	for(unsigned int x=0; x<spectro.size(); x++)
	{
		for(unsigned int y=0; y<spectro[x].size(); y++)
		{
			pixelColor(screen, x, y, ((int)(256*spectro[x][y]/max))<<16|0xFF);
		//	pixelColor(screen, x, y, ((int)(spectro[x][y]))<<16|0xFF);
		}
		SDL_Flip(screen);
	}
	SDL_Flip(screen);
    std::this_thread::sleep_for(dura);
	std::cout << max << std::endl;
}

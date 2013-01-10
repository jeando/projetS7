#include"MyAL.h"
#include"traitement.h"
#include<iostream>
#include<sstream>
#include<SDL/SDL.h>
#include<SDL/SDL_gfxPrimitives.h>
#include<SDL/SDL_image.h>
#include<sdl.h>
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
		samples_double.push_back(samples[i]);
		//samples_double.push_back(samples[i]/MAX_ALSHORT_VAL);
	}
	std::vector<double> dec = decoupage(samples_double,783,30*alc.getSampleRate()/1000);
	std::cout << dec.size() << std::endl;
/*	std::vector<double> signal_auto = autocorrelation(samples_double);
	std::vector<ALshort> tosave;
	for(std::vector<double>::iterator it = signal_auto.begin(); it!=signal_auto.end(); it++)
	{
		tosave.push_back(*it);
	}
	MyAL::save_sound("son_autocorreller.wav", tosave);
	//*/
	std::vector<std::vector<double> > spectro
		= spectrogramme(samples_double,alc.getSampleRate(),false);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen(nullptr);
	screen = SDL_SetVideoMode(spectro.size(),256,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
	for(unsigned int x=0; x<spectro.size(); x++)
	{
		draw_vect(screen, spectro[x]);	
    	std::this_thread::sleep_for(dura/32);
	}
	
	draw_mat(screen,spectro);
	SDL_SaveBMP(screen, "spectro_long.bmp");
    std::this_thread::sleep_for(dura);
	
	spectro = spectrogramme(samples_double,alc.getSampleRate());
	SDL_SaveBMP(screen, "spectro_court.bmp");
	draw_mat(screen,spectro);
    std::this_thread::sleep_for(dura);
}

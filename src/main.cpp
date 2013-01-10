#include"MyAL.h"
#include"traitement.h"
#include<iostream>
#include<sstream>
#include<string>
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
    std::this_thread::sleep_for(dura/10);
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
	std::vector<std::vector<double> > spectro
		= spectrogramme(samples_double,alc.getSampleRate(),false);

    std::chrono::milliseconds dura_2(600);
	//std::vector<std::string> vect_mot={"gauche","droite","haut","bas","diag","un","deux","trois","quatre","cinq","six"};
//	std::vector<std::string> vect_mot={"gauche","droite"};//,"haut","bas","diag"};//,"un","deux","trois","quatre","cinq","six"};
	std::vector<std::string> vect_mot={"gauche","gauche","droite","droite"};//,"haut","bas","diag"};//,"un","deux","trois","quatre","cinq","six"};
//	std::vector<std::string> vect_mot={"gauche","droite","haut","bas","diag"};//,"un","deux","trois","quatre","cinq","six"};
	std::vector<std::vector<std::vector<double> > > vect_spectro;
	std::vector<double> vect_distance;
	std::string tmp_str;
	for(std::string s : vect_mot)
	{
		std::cout << "dites le mot "<< s << std::endl;
		std::cin >> tmp_str;
		samples.clear();
    	alc.start(samples);
    	std::this_thread::sleep_for(dura_2);
    	alc.stop();
		std::cout << "fin enreg" << std::endl;
		samples_double.clear();
		for(unsigned int i=0; i<samples.size();i++){
			samples_double.push_back(samples[i]);
		}
		vect_spectro.push_back(spectrogramme(samples_double,alc.getSampleRate(),false));
	}
	std::string mot_a_tester;
	std::cout << "entrez le mot que vous voulez dire et dites le" << std::endl;
	while(tmp_str!="0"||tmp_str!="quit"||tmp_str!=":q"||tmp_str!="exit")
	{
		std::cout <<	"tappez  exit pour quitter, autre chose pour continuer" << std::endl;
		std::cin >> tmp_str;
		samples.clear();
    	alc.start(samples);
	    std::this_thread::sleep_for(dura_2);
    	alc.stop();
		samples_double.clear();
		for(unsigned int i=0; i<samples.size();i++){
			samples_double.push_back(samples[i]);
		}
		std::vector<std::vector<double> > spectro_a_tester
		= spectrogramme(samples_double,alc.getSampleRate(),false);
		vect_distance.clear();
		for(auto it = vect_spectro.begin(); it!=vect_spectro.end(); it++){
			vect_distance.push_back(distance(dynamic_time_warping(spectro_a_tester, *it,30)));
		}
		int min_val(vect_distance[0]);
		int min_indice(0);
		for(int i=0; i<vect_distance.size();i++){
			std::cout<<vect_mot[i]<<" : " << vect_distance[i] << std::endl;
			if(vect_distance[i]<min_val){
				min_val=vect_distance[i];
				min_indice=i;
			}
		}
		std::cout << "Vous avez dit le mot : " << vect_mot[min_indice]<<std::endl;
	}	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen(nullptr);
	screen = SDL_SetVideoMode(spectro.size(),256,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
	for(unsigned int x=0; x<spectro.size(); x++)
	{
		draw_vect(screen, spectro[x]);	
    	std::this_thread::sleep_for(dura/128);
	}
	
	draw_mat(screen,spectro);
	SDL_SaveBMP(screen, "spectro_long.bmp");
    std::this_thread::sleep_for(dura);
	
	spectro = spectrogramme(samples_double,alc.getSampleRate());
	draw_mat(screen,spectro);
	SDL_SaveBMP(screen, "spectro_court.bmp");
    std::this_thread::sleep_for(dura);



}

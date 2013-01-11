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
    //enregistrement:
    std::chrono::milliseconds dura(4000);
    std::vector<ALshort> samples;
    //enregistre
    AL_Capture alc(MyAL::choisir_device(),MyAL::choisir_capture_device());//port audio / micro
    alc.start(samples);
    //dors dura milliseconds
    //std::this_thread::sleep_for(dura/10);
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
//	std::vector<double> dec = decoupage(samples_double,783,30*alc.getSampleRate()/1000);
//	std::cout << dec.size() << std::endl;
	std::vector<std::vector<double> > spectro
		= spectrogramme(samples_double,alc.getSampleRate(),false);
	std::vector<std::vector<double> > spectro_court
		= spectrogramme(samples_double,alc.getSampleRate());

    std::chrono::milliseconds dura_2(1000);
	//std::vector<std::string> vect_mot={"gauche","droite","haut","bas","diag","un","deux","trois","quatre","cinq","six"};
//	std::vector<std::string> vect_mot={"gauche","droite"};//,"haut","bas","diag"};//,"un","deux","trois","quatre","cinq","six"};
	std::vector<std::string> vect_mot={
		"gauche",
		"gauche ",
		"droite",
		"droite ",
		"haut",
		"haut ",
		"bas",
		"bas "
	};//,"diag"};//,"un","deux","trois","quatre","cinq","six"};
//	std::vector<std::string> vect_mot={"gauche","droite","haut","bas","diag"};//,"un","deux","trois","quatre","cinq","six"};
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen(nullptr);
	screen = SDL_SetVideoMode(spectro.size(),256,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
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
		vect_spectro.push_back(equalize_spectrogramme(spectrogramme(samples_double,alc.getSampleRate())));
		draw_mat(screen,vect_spectro.back());
		std::ostringstream oss("");
		oss << "spectro_ref_" << s << ".bmp";
		SDL_SaveBMP(screen, oss.str().c_str());
		//indice_debut(vect_spectro.back());
	}
	std::string mot_a_tester;
	std::cout << "entrez le mot que vous voulez dire et dites le" << std::endl;
	int nb_test(0);
	while(tmp_str!="0"&&tmp_str!="quit"&&tmp_str!=":q"&&tmp_str!="exit")
	{
		nb_test++;
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
		= equalize_spectrogramme(spectrogramme(samples_double,alc.getSampleRate()));
		draw_mat(screen,spectro_a_tester);
		std::ostringstream oss("");
		oss << "spectro_test_" << nb_test << ".bmp";
		SDL_SaveBMP(screen, oss.str().c_str());
		std::cout << oss.str().c_str() << "ecrit" << std::endl;
		vect_distance.clear();
		for(auto it = vect_spectro.begin(); it!=vect_spectro.end(); it++){
			vect_distance.push_back(distance(dynamic_time_warping(spectro_a_tester, *it,10)));
		}
		double min_1_val(vect_distance[0]);
		int min_1_indice(0);
		double min_2_val(vect_distance[0]);
		int min_2_indice(0);
		for(unsigned int i=0; i<vect_distance.size();i++){
			std::cout<<vect_mot[i]<<" : " << vect_distance[i] << std::endl;
			if(vect_distance[i]<min_1_val){
				min_2_val = min_1_val;
				min_2_indice = min_1_indice;
				min_1_val=vect_distance[i];
				min_1_indice=i;
			}
		}
		double d1 = (distance(dynamic_time_warping(spectro_a_tester,
						vect_spectro[min_1_indice],10,0,spectro_a_tester.begin()->size()/2)));
		double d2 = (distance(dynamic_time_warping(spectro_a_tester,
						vect_spectro[min_2_indice],10,0,spectro_a_tester.begin()->size()/2))); 

		std::cout << d1 <<std::endl;
		std::cout << d2 <<std::endl;
		std::cout << "Vous avez dit le mot : " << vect_mot[min_1_indice]<<std::endl;
	}	
	for(unsigned int x=0; x<spectro.size(); x++)
	{
		draw_vect(screen, spectro[x]);	
    	std::this_thread::sleep_for(dura/128);
	}
	
	draw_mat(screen,spectro);
	SDL_SaveBMP(screen, "spectro_long.bmp");
    std::this_thread::sleep_for(dura);
	
	draw_mat(screen,spectro_court);
	SDL_SaveBMP(screen, "spectro_court.bmp");
    std::this_thread::sleep_for(dura);
	return EXIT_SUCCESS;
}

#include"../MyAL.h"
#include"../traitement.h"
#include"../sdl.h"
#include<iostream>
int main()
{
	std::chrono::time_point<std::chrono::system_clock> time_1;
	std::chrono::time_point<std::chrono::system_clock> time_2;
	std::vector<ALshort> samples;
	std::vector<std::pair<std::vector<ALshort>,SF_INFO> >samples_ref;
	samples_ref.push_back(MyAL::load_sound("gauche_ref.wav"));
	samples_ref.push_back(MyAL::load_sound("gauche_ref2.wav"));
	samples_ref.push_back(MyAL::load_sound("droite_ref.wav"));
	samples_ref.push_back(MyAL::load_sound("droite_ref2.wav"));
	samples_ref.push_back(MyAL::load_sound("haut_ref.wav"));
	samples_ref.push_back(MyAL::load_sound("haut_ref2.wav"));
	samples_ref.push_back(MyAL::load_sound("bas_ref.wav"));
	samples_ref.push_back(MyAL::load_sound("bas_ref2.wav"));
	std::string continuer("go");
    std::chrono::milliseconds dura(1000);
	AL_Capture alc(MyAL::choisir_device(),MyAL::choisir_capture_device());
	std::vector<std::vector<double> > spectro;
	unsigned int debut,size;
	std::vector<std::vector<double> > spectro2;
	std::vector<std::vector<double> > spectro_temp;
	std::vector<double> vdist0;
	std::vector<double> vdist;
	std::vector<double> vdist2;
	std::vector<double> vdist3;
	std::vector<double> temp;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen(nullptr);
	screen = SDL_SetVideoMode(1,1,32,
			 SDL_HWSURFACE | SDL_DOUBLEBUF);
	int min;
	int min_indice;
	int aaa;
	while(continuer!="q"&&continuer!="quit"
		&&continuer!="e"&&continuer!="exit"
		&&continuer!=":q"){
		samples.clear();
		vdist.clear();
		vdist0.clear();
		vdist2.clear();
		vdist3.clear();
	std::cout << "speak" << std::endl;
	alc.start(samples);
    std::this_thread::sleep_for(dura);
    alc.stop();
	std::cout << "end speak" << std::endl;

	alc.save_sound("test.wav");
	
	spectro2 =	spectro =  equalize_spectrogramme(spectrogramme(
								MyAL::sample_to_double(samples),
								alc.getSampleRate()));
	debut = indice_debut(spectro2);
	if(debut == -1){
		std::cout << "parler plus fort" << std::endl;
		std::cin >> continuer;
		continue;
	}
	size = spectro2[0].size();
	for(unsigned int i = 0; i<debut; i++)
	{
		for(unsigned int j=0; j<size; j++)
		{
			spectro[i].push_back(0);
		}
	}
	for(unsigned int i = debut; i<spectro.size(); i++)
	{
		for(unsigned int j=0; j<size; j++)
		{
			spectro[i].push_back(spectro2[i][j]);
		}
	}
	std::cout << "start analyse" << std::endl;
	for(auto a : samples_ref){
		spectro_temp = equalize_spectrogramme(spectrogramme(MyAL::sample_to_double(a.first), a.second.samplerate));
		debut = indice_debut(spectro_temp);
		size = spectro_temp[0].size();
		for(unsigned int i = 0; i<debut; i++)
		{
			for(unsigned int j=0; j<size; j++)
			{
				spectro[i].push_back(0);
			}
		}
		for(unsigned int i = debut; i<spectro.size(); i++)
		{
			for(unsigned int j=0; j<size; j++)
			{
				spectro[i].push_back(spectro_temp[i][j]);
			}
		}
		time_1 = std::chrono::system_clock::now();
		vdist0.push_back(distance(dynamic_time_warping0(spectro2,spectro_temp,3)));
		time_2 = std::chrono::system_clock::now();
		std::cout << "temps dtw0: " << (time_2-time_1).count() << std::endl;
		time_1 = std::chrono::system_clock::now();
		
		vdist.push_back(distance(dynamic_time_warping(spectro2,spectro_temp,3)));
		time_2 = std::chrono::system_clock::now();
		std::cout << "temps dtw1: " << (time_2-time_1).count() << std::endl;
		time_1 = std::chrono::system_clock::now();
		
		vdist2.push_back(distance(dynamic_time_warping2(spectro2,spectro_temp,3)));
		time_2 = std::chrono::system_clock::now();
		std::cout << "temps dtw2: " << (time_2-time_1).count() << std::endl;
		time_1 = std::chrono::system_clock::now();
	
		temp=get_min_dist_spectre(spectro2,spectro_temp,3,0,0,spectro2.size());
		vdist3.push_back(*std::min_element(temp.begin(), temp.end()));
		time_2 = std::chrono::system_clock::now();
		std::cout << "temps dtw3: " << (time_2-time_1).count() << std::endl;
	}
	draw_mat(screen, spectro,8,4);
	aaa=0;
aaa_aaa:
	min=vdist[0];
	min_indice=0;
	for(unsigned int i=0; i<vdist.size(); i++)
	{
		//std::cout << vdist[i] << std::endl;
		if(vdist[i]<min){
			min=vdist[i];
			min_indice=i;
		}
	}
	switch(min_indice)
	{
		case 0:
		case 1:
			std::cout << "gauche" << std::endl;
			break;
		case 2:
		case 3:
			std::cout << "droite" << std::endl;
			break;
		case 4:
		case 5:
			std::cout << "haut" << std::endl;
			break;
		case 6:
		case 7:
			std::cout << "bas" << std::endl;
			break;
	}
	if(aaa==0)
	{
		aaa++;
		vdist=vdist0;
		goto aaa_aaa;
	}
	else if(aaa==1)
	{
		aaa++;
		vdist=vdist2;
		goto aaa_aaa;
	}
	else if(aaa==2)
	{
		aaa++;
		vdist=vdist3;
		goto aaa_aaa;
	}
	std::cin >> continuer;
	}
	SDL_SaveBMP(screen, "test_detect_debut.bmp");
	return EXIT_SUCCESS;
}

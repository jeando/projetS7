#include"../MyAL.h"
#include"../traitement.h"
#include"../sdl.h"
#include<iostream>
int main()
{
	std::vector<ALshort> samples;
	std::vector<std::pair<std::vector<ALshort>,SF_INFO> >samples_ref;
	samples_ref.push_back(MyAL::load_sound("gauche_ref.wav"));
	samples_ref.push_back(MyAL::load_sound("droite_ref.wav"));
	samples_ref.push_back(MyAL::load_sound("haut_ref.wav"));
	samples_ref.push_back(MyAL::load_sound("bas_ref.wav"));

	AL_Capture alc(MyAL::choisir_device(),MyAL::choisir_capture_device());
	alc.start(samples);
    std::chrono::milliseconds dura(1000);
    std::this_thread::sleep_for(dura);
    alc.stop();

	alc.save_sound("test.wav");

	std::vector<std::vector<double> > spectro =  equalize_spectrogramme(spectrogramme(MyAL::sample_to_double(samples),
				alc.getSampleRate()));
	std::vector<std::vector<double> > spectro2 = spectro;
	std::vector<std::vector<double> > spectro_temp;
	unsigned int debut = indice_debut(spectro2);
	unsigned int size = spectro2[0].size();
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
	std::vector<double> vdist;
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
		vdist.push_back(distance(dynamic_time_warping(spectro2,spectro_temp,5)));
	}
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen(nullptr);
	screen = SDL_SetVideoMode(1,1,32,
			 SDL_HWSURFACE | SDL_DOUBLEBUF);
	draw_mat(screen, spectro,8,4);
	int min(vdist[0]);
	int min_indice(0);
	for(unsigned int i=0; i<vdist.size(); i++)
	{
		std::cout << vdist[i] << std::endl;
		if(vdist[i]<min){
			min=vdist[i];
			min_indice=i;
		}
	}
	switch(min_indice)
	{
		case 0:
			std::cout << "gauche" << std::endl;
			break;
		case 1:
			std::cout << "doite" << std::endl;
			break;
		case 2:
			std::cout << "haut" << std::endl;
			break;
		case 3:
			std::cout << "bas" << std::endl;
			break;
	}
	std::string t;
	std::cin >> t;
	SDL_SaveBMP(screen, "test_detect_debut.bmp");
	return EXIT_SUCCESS;
}

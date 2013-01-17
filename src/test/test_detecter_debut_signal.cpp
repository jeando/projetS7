#include"../traitement.h"
#include"../MyAL.h"
#include"../sdl.h"
#include<iostream>

int main(int argc, char** argv)
{
	std::pair<std::vector<ALshort>,SF_INFO> samples;
	if(argc<2)
	{
		samples = MyAL::load_sound("gauche.wav");
	}
	else
	{
		samples = MyAL::load_sound(argv[1]);
	}
	std::vector<std::vector<double> > spectro =
		spectrogramme(MyAL::sample_to_double(samples.first),
				samples.second.samplerate);
	int debut = indice_debut(spectro);
	unsigned int size = spectro[0].size();
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
			spectro[i].push_back(spectro[i][j]);
		}
	}
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen(nullptr);
	screen = SDL_SetVideoMode(1,1,32,
			 SDL_HWSURFACE | SDL_DOUBLEBUF);
	draw_mat(screen, spectro);
	system("sleep 10");
	return EXIT_SUCCESS;
}

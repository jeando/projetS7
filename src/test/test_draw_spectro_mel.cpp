#include"../MyAL.h"
#include"../traitement.h"
#include"../sdl.h"
int main(int argc, char** argv)
{
	std::pair<std::vector<ALshort>,SF_INFO> samples;
	if(argc<2)
		samples = MyAL::load_sound("../gdgd.wav");
	else
		samples = MyAL::load_sound(argv[1]);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* screen(nullptr);
	screen = SDL_SetVideoMode(1,1,32,
		SDL_HWSURFACE | SDL_DOUBLEBUF);
	draw_mat(screen, spectrogramme(MyAL::sample_to_double(samples.first), samples.second.samplerate));
	SDL_SaveBMP(screen, "test_spectro_mel");
	system("sleep 10");
	return EXIT_SUCCESS;
}

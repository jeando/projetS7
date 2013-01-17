#include"../MyAL.h"
#include"../traitement.h"
#include"../sdl.h"
#include<iostream>
int main(int argc, char** argv)
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

	std::vector<std::vector<double> > spectro =  spectrogramme(MyAL::sample_to_double(samples), alc.getSampleRate());
	std::vector<double> vdist;
	for(auto a : samples_ref){
		vdist.push_back(distance(dynamic_time_warping(spectro,
					spectrogramme(MyAL::sample_to_double(
							a.first), a.second.samplerate))));
		std::cout << vdist.back() << std::endl;
	}
	int min(vdist[0]);
	int min_indice(0);
	for(unsigned int i=1; i<vdist.size(); i++)
	{
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
	return EXIT_SUCCESS;
}

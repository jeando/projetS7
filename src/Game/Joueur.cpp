#include "Joueur.h"
#include"../traitement.h"
#include"../MyAL.h"
#include<sstream>
using namespace std;
Joueur::Joueur(int i, string nom)
:nom_joueur(nom)
{
	std::ostringstream oss("");
	oss << i;
	id = oss.str();
    charger();
}

void Joueur::charger()
{
	std::cout << id << std::endl;
    std::pair<std::vector<ALshort>,SF_INFO> samples;
	samples =  MyAL::load_sound("data/"+id+"_gauche_1.wav");
	spectro_gauche1 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+id+"_gauche_2.wav");
	spectro_gauche2 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+id+"_droite_1.wav");
	spectro_droite1 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+id+"_droite_2.wav");
	spectro_droite2 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+id+"_haut_1.wav");
	spectro_haut1 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+id+"_haut_2.wav");
	spectro_haut2 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+id+"_bas_1.wav");
	spectro_bas1 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+id+"_bas_2.wav");
	spectro_bas2 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+id+"_tir_1.wav");
	spectro_tir1 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+id+"_tir_2.wav");
	spectro_tir2 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
}

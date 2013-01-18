#include "Joueur.h"
#include"../traitement.h"
#include"../MyAL.h"
using namespace std;
Joueur::Joueur(string nom)
:nom_joueur(nom)
{
    charger();
}

void Joueur::charger()
{
    std::pair<std::vector<ALshort>,SF_INFO> samples;
	samples =  MyAL::load_sound("data/"+nom_joueur+"_gauche_1.wav");
	spectro_gauche1 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+nom_joueur+"_gauche_2.wav");
	spectro_gauche2 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+nom_joueur+"_droite_1.wav");
	spectro_droite1 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+nom_joueur+"_droite_2.wav");
	spectro_droite2 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+nom_joueur+"_haut_1.wav");
	spectro_haut1 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+nom_joueur+"_haut_2.wav");
	spectro_haut2 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+nom_joueur+"_bas_1.wav");
	spectro_bas1 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
	samples =  MyAL::load_sound("data/"+nom_joueur+"_bas_2.wav");
	spectro_bas2 = equalize_spectrogramme(spectrogramme(
				MyAL::sample_to_double(samples.first),
				samples.second.samplerate));
}

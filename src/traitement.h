#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include<vector>
#include"util_tmp.h"
#include"ft.h"
#include"event_audio.h"
#include"Game/Joueur.h"
events_audio analyse(std::vector<std::vector<double> > spectro, Joueur& joueur);

std::vector<double> autocorrelation(std::vector<double> input);
std::vector<double> decoupage(std::vector<double> input, unsigned int t0, const unsigned int DELTA);

std::vector<double> amplitude(std::vector<std::complex<double> > input);
std::vector<std::vector<double> > spectrogramme(std::vector<double> input, int sample_rate,bool with_mel = true);
std::vector<double> echelle_mel(std::vector<double> input, int sample_rate);
/*!
 * decale de v.size()/2 le signal v
 */
std::vector<std::vector<double> > filtre_gaussien(
		std::vector<std::vector<double> > spectro, int size = 3);
void decaler(std::vector<double>& v);
std::vector<std::vector<double> > equalize_spectrogramme(
		std::vector<std::vector<double> > spectro,
		double val_min = 0, double val_max = 255);


std::vector<double> dynamic_time_warping(
		std::vector<std::vector<double> > mesure,
		std::vector<std::vector<double> > ref,unsigned int delta=10);
std::vector<double> dynamic_time_warping(
		std::vector<std::vector<double> > mesure,
		std::vector<std::vector<double> > ref,unsigned int delta,
		unsigned int indice_debut, unsigned int indice_fin);

int indice_debut(std::vector<std::vector<double> > spectro);
//templates fonctions	
template<typename T>
T distance(std::vector<T> x, unsigned int indice_debut, unsigned int indice_fin)
{
	T distance(0);
	for(unsigned int i = indice_debut; i<indice_fin; i++)
	{
		distance+=(pow(x[i],2));
//		distance+=fabs(x[i]);
	}
	return distance;
}
template<typename T>
T distance(std::vector<T> x)
{
	return distance(x,0,static_cast<int>(x.size()));
}
template<typename T>
inline T distance(std::vector<T>& x, std::vector<T>& y, unsigned int indice_debut, unsigned int indice_fin)
{
	T distance(0);
	for(unsigned int i = indice_debut; i<indice_fin; i++)
	{
		distance+=(pow(x[i]-y[i],2));
	}
	return distance;
}
template<typename T>
inline T distance(std::vector<T>& x, std::vector<T>& y)
{
	return distance(x,y,0,static_cast<int>(x.size()));
}
#endif


#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include<vector>
#include"util_tmp.h"
#include"ft.h"
#include"event_audio.h"
#include"Game/Joueur.h"

inline std::vector<std::complex<double> > demi_signal(const std::vector<std::complex<double> >& input);
std::vector<double> dynamic_time_warping4(
		const std::vector<std::vector<double> >& mesure,
		const std::vector<std::vector<double> >& ref,unsigned int delta=3);
std::vector<double> dynamic_time_warping0(
		std::vector<std::vector<double> > mesure,
		std::vector<std::vector<double> > ref,unsigned int delta= 3);
std::vector<double> dynamic_time_warping0(
		std::vector<std::vector<double> > mesure,
		std::vector<std::vector<double> > ref,unsigned int delta,
		unsigned int _indice_debut_f, unsigned int _indice_fin_f);
std::vector<double> dynamic_time_warping2(
		std::vector<std::vector<double> > mesure,
		std::vector<std::vector<double> > ref,unsigned int delta=3, int prof =20);
std::vector<double> dynamic_time_warping2(
		std::vector<std::vector<double> > mesure,
		std::vector<std::vector<double> > ref,unsigned int delta,
		unsigned int _indice_debut_f, unsigned int _indice_fin_f,int prof);
inline std::vector<double> get_min_dist_spectre(
		const std::vector<std::vector<double> >& mesure,
		const std::vector<std::vector<double> >& ref,
		const std::vector<double>& min_suivant,
		unsigned int delta,
		unsigned int n,
		unsigned int m,
		unsigned int profondeur,
		unsigned int profondeur_total);
inline std::vector<double> get_min_dist_spectre(
		const std::vector<std::vector<double> >& mesure,
		const std::vector<std::vector<double> >& ref,
		unsigned int delta,
		unsigned int n,
		unsigned int m,
		unsigned int profondeur = 3);
inline std::vector<double> max_f(std::vector<std::vector<double> >& spectro);
events_audio analyse(std::vector<std::vector<double> > spectro, Joueur& joueur);

std::vector<double> autocorrelation(std::vector<double> input);
std::vector<double> decoupage(const std::vector<double>& input, unsigned int t0, const unsigned int DELTA);

std::vector<double> amplitude(const std::vector<std::complex<double> >& input);
inline std::vector<double> amplitude_pond_log(const std::vector<std::complex<double> >& input);
std::vector<std::vector<double> > spectrogramme(std::vector<double> input, int sample_rate,bool with_mel = true);
inline std::vector<double> echelle_mel(const std::vector<double>& input, int sample_rate);
/*!
 * decale de v.size()/2 le signal v
 */
std::vector<std::vector<double> > filtre_gaussien(
		const std::vector<std::vector<double> >& spectro, int size = 3);
void decaler(std::vector<double>& v);
std::vector<std::vector<double> > equalize_spectrogramme(
		const std::vector<std::vector<double> >& spectro,
		double val_min = 0, double val_max = 255);


std::vector<double> dynamic_time_warping(
		const std::vector<std::vector<double> >& mesure,
		const std::vector<std::vector<double> >& ref,unsigned int delta=3);
inline std::vector<double> dynamic_time_warping(
		const std::vector<std::vector<double> >& mesure,
		const std::vector<std::vector<double> >& ref,unsigned int delta,
		unsigned int indice_debut, unsigned int indice_fin);

int indice_debut(std::vector<std::vector<double> > spectro);
//templates fonctions	
template<typename T>
T distance(const std::vector<T>& x, unsigned int indice_debut, unsigned int indice_fin)
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
T distance(const std::vector<T>& x)
{
	return distance(x,0,static_cast<int>(x.size()));
}
template<typename T>
inline T distance(const std::vector<T>& x, const std::vector<T>& y, unsigned int indice_debut, unsigned int indice_fin)
{
	T distance(0);
	for(unsigned int i = indice_debut; i<indice_fin; i++)
	{
		distance+=(pow(x[i]-y[i],2));
	}
	return distance;
}
template<typename T>
inline T distance(const std::vector<T>& x,const std::vector<T>& y)
{
	return distance(x,y,0,static_cast<int>(x.size()));
}
#endif


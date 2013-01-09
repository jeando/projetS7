#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include<vector>

#include"ft.h"

std::vector<double> autocorrelation(std::vector<double> input);
std::vector<double> decoupage(std::vector<double> input, unsigned int t0, const unsigned int DELTA);

std::vector<double> amplitude(std::vector<std::complex<double> > input);
std::vector<std::vector<double> > spectrogramme(std::vector<double> input, int sample_rate,bool with_mel = true);
std::vector<double> echelle_mel(std::vector<double> input, int sample_rate);
/*!
 * decale de v.size()/2 le signal v
 */
void decaler(std::vector<double>& v);
#endif


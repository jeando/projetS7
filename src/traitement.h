#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include<vector>

#include"ft.h"

std::vector<double> autocorrelation(std::vector<double> input);
std::vector<double> decoupage(std::vector<double> input, unsigned int t0, const unsigned int DELTA);

std::vector<std::vector<double> > spectrogramme(std::vector<double> input);

#endif

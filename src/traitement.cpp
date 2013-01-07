#include"traitement.h"
#include<cmath>

/**
*fonction autocorrelation: permet de lisser la courbe
*/
std::vector<double> autocorrelation(std::vector<double> input)
{
	std::vector<double> output;
	double temp;
    for(unsigned int tau = 0; tau<input.size(); tau++)
    {
        temp = 0;
        for(unsigned t = tau; t<input.size(); t++)
        {
            temp+=input[t]*input[t-tau];
        }
        output.push_back(temp);
    }
	return output;
}

/**
* decoupe le signal en une bande de taille delta et de centre t0
* Et rajoute des 0 pour aller a la puissance de 2 superieure la plus proche
*/
template<int DELTA>
std::vector<double> decoupage(std::vector<double> input, unsigned int t0)
{
	std::vector<double> output;
	unsigned int t = t0 - DELTA;
	unsigned int tmax = t0 + DELTA;

	// si les bornes despassent du signal d'entree ==> on verifie les problemes aux limites
	if(t<0)t=0;
	if(tmax>input.size()-1)tmax=input.size()-1;

	//recupere la bande qui nous interesse
	for(; t<tmax;t++){
		output.push_back(input[t]);
	}

	//rajoute les 0 pour aller a la puissance de 2 superieure la plus proche
	double puissanceDe2 = log2(2*DELTA+1);
	int p2 = ceil(puissanceDe2);
	int taille=pow(p2,2);
	for(unsigned int k=input.size(); k<taille;k++)
	{
		output.push_back(0);
	}
	return output;
}
std::vector<std::vector<double> > spectrogramme(std::vector<double> input)
{
	std::vector<std::vector<double> > output;
	return output;
}

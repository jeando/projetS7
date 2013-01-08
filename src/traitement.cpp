#include"traitement.h"
#include<cmath>
#include<iostream>
#include<algorithm>
void decaler(std::vector<double>& v)
{
    double temp;
    for(unsigned int i=0;i<v.size()/2;i++){
        temp=v[i+v.size()/2];
        v[i+v.size()/2]=v[i];
        v[i]=temp;
    }
}
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
* decoupe le signal en une bande de taille delta et de debut t0
*/
std::vector<double> decoupage(std::vector<double> input, unsigned int t0, const unsigned int DELTA)
{
	std::vector<double> output;
	int tt = t0;
	unsigned int tmax = t0 + DELTA;
	double puissanceDe2 = log2(DELTA);
	int p2 = ceil(puissanceDe2);
	unsigned int taille=pow(2,p2);
	// si les bornes despassent du signal d'entree ==> on verifie les problemes aux limites
	tmax = tt + taille;
	while(tmax>input.size()-1)
	{
		taille/=2;
		tmax=tt+taille;
	}

	//recupere la bande qui nous interesse
	for(unsigned int t = tt; t<tmax;t++){
		output.push_back(input[t]);
	}

	return output;
}
std::vector<double> amplitude_pond_log(std::vector<std::complex<double> > input)
{
	std::vector<double> output;
	for(unsigned int i=0; i<input.size(); i++)
	{
		output.push_back(log10(0.0000000001+sqrt(std::norm(input[i]))*pow(M_PI*i/input.size(),0.6)));//non nul / recuperer l'amplitude de la frq cmpl / ponderation cf ppt => negliger frequences basses / log ?
	}
	return output;
}
std::vector<double> amplitude(std::vector<std::complex<double> > input)
{
	std::vector<double> output;
	for(std::vector<std::complex<double> >::iterator it
			= input.begin(); it != input.end(); it++)
	{
		output.push_back(sqrt(std::norm(*it)));
	}
	return output;
}
std::vector<double> echelle_mel(std::vector<double> input, int sample_rate)
{
	std::vector<double> output={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int frqech_sur_2 = sample_rate/2;
	for(unsigned int i=input.size()/2; i<input.size(); i++)
	{
		unsigned int k = i-input.size();
	//	output[]
	}
	return output;
}

/**
* pour eviter les grosses discontinuites aux bords
*/
void fenetre_hamming(std::vector<double> input)
{
	for(unsigned int i=0; i<input.size(); i++)
	{
		input[i]*=(0.54-0.46*cos(2*M_PI*i/(input.size()-1)));//coeff minimal aux bords, max au centre
	}
}

/**
*
*/
std::vector<std::vector<double> > spectrogramme(std::vector<double> input, int sample_rate)
{
	std::vector<std::vector<double> > output;
	input = autocorrelation(input);
	double puissanceDe2 = log2(sample_rate*30/1000);
	int p2 = ceil(puissanceDe2);
	unsigned int taille=pow(2,p2);
	int i = input.size()%taille;
	//on rajoute des points pour arriver a un multiple du nombre de tranches, qui soit une puissance de 2 ==> nbr de tranches entier (on complete juste la derniere tranche)
	for(int k=0; k<i; k++)
	{
		input.push_back(0);
	}

	//pour chaque tranche
	for(std::vector<double>::iterator it = input.begin();
			std::distance(it,input.end())>0; it+=taille/2)
	{
		std::vector<double> tmp(taille);
		std::copy(it, it+taille, tmp.begin());//decoupage / selection tranche
		fenetre_hamming(tmp);//pas grave car recoupement
		tmp=amplitude_pond_log(fft(tmp));

		//	= amplitude(fft(decoupage(inputautocor, *it, sample_rate*30/1000)));
		//decaler(tmp);//centre en 0


	//	echelle_mel(tmp,sample_rate);
		
		output.push_back(tmp);
	}
	return output;
}

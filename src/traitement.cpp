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
		output.push_back(log(0.0000000001+sqrt(std::norm(input[i]))*pow(M_PI*i/input.size(),0.6)));//non nul / recuperer l'amplitude de la frq cmpl / ponderation cf ppt => negliger frequences basses / log ?
	}
	std::cout << output[input.size()/2] << std::endl;
	return output;
}
std::vector<std::complex<double> > demi_signal(std::vector<std::complex<double> > input)
{
	std::vector<std::complex<double> > output;
	for(unsigned int i=0; i<input.size()/2; i++)
	{
		output.push_back(input[i]);
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
#define i_en_Hz(i) (i * sample_rate / ( 2 * input.size() ))

/*
inline unsigned int i_en_hz(unsigned int i)
{
	return i * sample_rate / ( 2 * input.size() );
}//*/
#define mel(i) (1000*log(1+i/1000)/log(2))
std::vector<double> echelle_mel(std::vector<double> input, int sample_rate)
{


	/*
TO_DO

log(mel(

	   */
	std::vector<double> output={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int frqech_sur_2 = sample_rate/2;
	std::vector<unsigned int> coeff_mel;
	//calcul des coeff
	for(int k=1; k<10; k++)//debut lineaire
	{
		coeff_mel.push_back(k*100);
	}
	for(int k=10; k<21; k++)//fin exponentielle
	{
		//coeff_mel.push_back((k-10)*400-(k-20)*100);
		coeff_mel.push_back(1000*pow(M_E,0.2*M_LN2*(k-10)));
	}
	unsigned int i = 0;
	unsigned int i_precedant_1 = 0;
	unsigned int i_precedant_2 = 0;
	for(; i_en_Hz(i)<coeff_mel[0]; i++)
	{
		output[0]+=input[i]*(i_en_Hz(i))/(coeff_mel[0]);
	}
	i_precedant_1=i;
	//application des coeff sur l'entree
	for(int j = 1; j< coeff_mel.size(); j++)
	{
		for(; i_en_Hz(i)<coeff_mel[j]; i++)
		{
			
			output[j]+=input[i]*(i_en_Hz(i)-coeff_mel[j-1])/(coeff_mel[j]-coeff_mel[j-1]);//triangle croiss
			output[j-1]+=input[i]*(1-(i_en_Hz(i)-coeff_mel[j-1])/(coeff_mel[j]-coeff_mel[j-1]));//decroiss
		}
		output[j-1]/=(i-i_precedant_2);
		i_precedant_2=i_precedant_1;
		i_precedant_1=i;
	}
	output[output.size()-1]/=(i_precedant_1-i_precedant_2);
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
template<typename T>
T distance(std::vector<T> x, std::vector<T> y)
{
	T distance(0);
	for(unsigned int i = 0; i<x.size(); i++)
	{
		distance+=(pow(x[i]-y[i],2));
	}
	return distance;
}
std::vector<double> dynamic_time_warping(
		std::vector<std::vector<double> > mesure,
		std::vector<std::vector<double> > ref, int delta)
{
	std::vector<double> dist;//(mesure.size());
	int n(0);//mesure iterateur
	int m(0);//ref iterateur
	for(;n<mesure.size(); n++)
	{
		double min((1<<((sizeof(int)<<3)-1))-1);// le plus grand int
		double tmp_dst;
		int indice_min;
		for(int k = (m-delta<0?0:(m-delta)); k< m+delta && k<ref.size(); k++)
		{
			tmp_dst = distance(mesure[n],ref[k]);
			if(tmp_dst<min)
			{
				min = tmp_dst;
				indice_min = k;
			}
		}
		m=indice_min;
		dist.push_back(sqrt(min));
	}
	return dist;
}


/**
*
*/
std::vector<std::vector<double> > spectrogramme(std::vector<double> input, int sample_rate, bool with_mel)
{
	std::vector<std::vector<double> > output;
//	input = autocorrelation(input);
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
		tmp=amplitude_pond_log(demi_signal(fft(tmp)));
		if(with_mel)
		{
			tmp=echelle_mel(tmp, sample_rate);
		}
		//	= amplitude(fft(decoupage(inputautocor, *it, sample_rate*30/1000)));
		//decaler(tmp);//centre en 0


	//	echelle_mel(tmp,sample_rate);
		
		output.push_back(tmp);
	}
	return output;
}

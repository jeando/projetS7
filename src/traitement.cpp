#include"traitement.h"
#include<cmath>
#include<iostream>
#include<algorithm>
#include<limits>
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
//	std::cout << output[input.size()/2] << std::endl;
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
	double coef_pour_mel = log(sample_rate/2000.0)/10.0;
	std::vector<unsigned int> coeff_mel;
	//calcul des coeff
	for(int k=1; k<10; k++)//debut lineaire jusqu a 1000 Hz
	{
		coeff_mel.push_back(k*100);
	}
	for(int k=10; k<21; k++)//fin exponentielle
	{
//		coeff_mel.push_back(1000*pow(M_E,0.2*M_LN2*(k-10)));
		coeff_mel.push_back(1000*pow(M_E,coef_pour_mel*(k-10)));
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
	for(unsigned int j = 1; j< coeff_mel.size(); j++)
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
std::vector<double> dynamic_time_warping(
		std::vector<std::vector<double> > mesure,
		std::vector<std::vector<double> > ref, int delta)
{
	std::vector<double> dist;//(mesure.size());
	unsigned int n(0);//mesure iterateur
	unsigned int m(0);//ref iterateur
	for(;n<mesure.size(); n++)
	{
		double min(std::numeric_limits<double>::max());
		double tmp_dst;
		unsigned int indice_min;
		for(unsigned int k = (m<delta?0:(m-delta)); k< m+delta && k<ref.size(); k++)
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
//		dist.push_back(sqrt(min*sqrt(distance(ref[m]))));
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
	/*
	//for(int k=0; k<i; k++)
	for(int k=0; k<taille-i; k++)
	{
		input.push_back(0);
	}
	//*/
	input.resize(input.size()-i);	
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
std::vector<std::vector<double> > equalize_spectrogramme(
		std::vector<std::vector<double> > spectro,
		double val_min, double val_max)
{
	double max = get_max_val(spectro);
	double min = get_min_val(spectro);
	std::vector<std::vector<double> > spectro2(spectro.size());
	for(std::vector<std::vector<double> >::iterator
			it1=spectro.begin(), it2=spectro2.begin(); it1!=spectro.end(); it1++, it2++)
	{
		*it2=std::vector<double>(it1->size());
		for(std::vector<double>::iterator it_1 = it1->begin(), it_2=it2->begin(); it_1!=it1->end(); it_1++, it_2++)
		{
//			*it_2=val_max * (*it_1) / max;
			*it_2=(*it_1-min) * (val_max - val_min) /( max - min) + val_min;
		}
	}
	return spectro2;

}
unsigned int indice_debut(std::vector<std::vector<double> > spectro)
{
	std::vector<double> norme;
	for(std::vector<std::vector<double> >::iterator it = spectro.begin();
			it!=spectro.end();it++)
	{
		norme.push_back(sqrt(distance(*it)));
		//norme.push_back(distance(*it));
		//std::cout << norme.back() << std::endl;
	}
	std::vector<double> norme_2;
	norme_2.push_back(2*norme[0]+norme[1]);
	for(unsigned int i=1; i<norme.size()-1; i++)
	{
		norme_2.push_back((norme[i-1]+norme[i]+norme[i+1])/3);
	}
	norme_2.push_back(2*norme[norme.size()-1]+norme[norme.size()-2]);
	norme=norme_2;
	norme_2.clear();
	norme_2.push_back(0);
	for(unsigned int i=1; i<norme.size()-1; i++)
	{
		norme_2.push_back(norme[i-1]-2*norme[i]+norme[i+1]);
//		norme_2.push_back(norme[i]-norme[i-1]);
		std::cout << norme_2.back() << std::endl;
	}
	
}

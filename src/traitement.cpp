#include"traitement.h"
#include<cmath>
#include<iostream>
#include<algorithm>
#include<limits>

events_audio analyse(std::vector<std::vector<double> > spectro, Joueur& joueur)
{
	std::vector<double> vdist;
	vdist.push_back(distance(
				dynamic_time_warping(spectro,joueur.spectro_gauche1)));
	vdist.push_back(distance(
				dynamic_time_warping(spectro,joueur.spectro_gauche2)));
	vdist.push_back(distance(
				dynamic_time_warping(spectro,joueur.spectro_droite1)));
	vdist.push_back(distance(
				dynamic_time_warping(spectro,joueur.spectro_droite2)));
	vdist.push_back(distance(
				dynamic_time_warping(spectro,joueur.spectro_haut1)));
	vdist.push_back(distance(
				dynamic_time_warping(spectro,joueur.spectro_haut2)));
	vdist.push_back(distance(
				dynamic_time_warping(spectro,joueur.spectro_bas1)));
	vdist.push_back(distance(
				dynamic_time_warping(spectro,joueur.spectro_bas2)));
	double min_val(vdist[0]);
	int min_indice(0);
//	std::cout << "azertyuiop" << std::endl;
	for(unsigned int i=0; i<vdist.size(); i++)
	{
		if(vdist[i]<min_val){
			min_val=vdist[i];
			min_indice=i;
	//		std::cout << "aaazzz" <<min_indice << std::endl;
		}
	//	std::cout << vdist[i] << std::endl;
	}
	std::cout << "aaa" <<min_indice << std::endl;
//	std::cout << "aaa" <<vdist.size() << std::endl;
//	std::cout << min_indice << std::endl;
//	std::cout << min_indice << std::endl;
	switch(min_indice)
	{
		case 0:
			return SON_GAUCHE;
		case 1:
			return SON_GAUCHE;
		case 2:
			return SON_DROITE;
		case 3:
			return SON_DROITE;
		case 4:
			return SON_HAUT;
		case 5:
			return SON_HAUT;
		case 6:
			return SON_BAS;
		default:
			return SON_BAS;
	}
}



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
		std::vector<std::vector<double> > ref,unsigned int delta)
{
	return dynamic_time_warping(mesure, ref, delta, 0, ref.begin()->size());
}
std::vector<double> dynamic_time_warping(
		std::vector<std::vector<double> > mesure,
		std::vector<std::vector<double> > ref,unsigned int delta,
		unsigned int _indice_debut, unsigned int _indice_fin)
{
	std::vector<double> dist;//(mesure.size());
	unsigned int n_ini(indice_debut(mesure));//mesure iterateur
	unsigned int m_ini(indice_debut(ref));//ref iterateur
	unsigned int n_end=mesure.size();//mesure iterateur
	unsigned int m_end=ref.size();//ref iterateur
	unsigned int n=n_ini;
	unsigned int m=m_ini;
	//std::cout << m_ini << " " << n_ini << " azertyuiop" <<std::endl;
	double v_moy = (n_end-n_ini)/(m_end-m_ini);
	for(;n<n_end; n++)
	{
		double min(std::numeric_limits<double>::max());
		double tmp_dst;
		unsigned int indice_min;
		m+=static_cast<int>(floor(v_moy));
		for(unsigned int k = (m<delta+m_ini?m_ini:(m-delta)); k< m+delta && k<m_end; k++)
		{
			tmp_dst = distance(mesure[n],ref[k],_indice_debut, _indice_fin);
//			if(pow(tmp_dst,0.5)*pow((1+abs(m-k))<min,0.25))
			if(tmp_dst<min)
			{
				min = tmp_dst;
				indice_min = k;
			}
		}
		if(min == std::numeric_limits<double>::max())
		{
			std::cout << v_moy << std::endl;
			return dist;
		}
		//dist.push_back(abs(indice_min-m));
//		dist.push_back(sqrt(min)/5*abs(indice_min-m));

		
		//dist.push_back(pow(min,0.5)*pow(1+abs(indice_min-m),0.25));


		m=indice_min;
//		if(m+v_moy+delta>m_end)
//			break;
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
//		tmp=amplitude_pond_log((fft(tmp)));
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
	double max = get_max_val(filtre_gaussien(spectro,3));
	double min = get_min_val(filtre_gaussien(spectro,3));
	std::vector<std::vector<double> > spectro2(spectro.size());
	for(std::vector<std::vector<double> >::iterator
			it1=spectro.begin(), it2=spectro2.begin(); it1!=spectro.end(); it1++, it2++)
	{
		*it2=std::vector<double>(it1->size());
		for(std::vector<double>::iterator it_1 = it1->begin(), it_2=it2->begin(); it_1!=it1->end(); it_1++, it_2++)
		{
//			*it_2=val_max * (*it_1) / max;
			*it_2=(*it_1-min) * (val_max - val_min) /( max - min) + val_min;
			if(*it_2>val_max)
				*it_2=val_max;
			if(*it_2<val_min)
				*it_2=val_min;
		}
	}
	return spectro2;

}

std::vector<std::vector<double> > filtre_gaussien(std::vector<std::vector<double> > spectro, int size)
{
	std::vector<std::vector<double> > spectro2;
	spectro2.push_back(std::vector<double>(spectro[0].size()));
	spectro2.push_back(std::vector<double>(spectro[1].size()));
	for(unsigned int x=1; x<spectro.size()-1; x++)
	{
		spectro2.push_back(std::vector<double>(spectro[x+1].size()));
		spectro2[x][0]=spectro2[x][spectro2[x].size()-1]=0;
		for(unsigned int y=1; y<spectro[x].size()-1; y++)
		{
			if(size==3)
			{
			//gaussienne
			// 1 2 1
			// 2 4 2 //milieu : 3 ou 4 ?
			// 1 2 1
				spectro2[x][y]=spectro[x-1][y+1]+2*spectro[x-1][y]+spectro[x-1][y-1];
				spectro2[x][y]+=2*spectro[x][y+1]+4*spectro[x][y]+2*spectro[x][y-1];
				spectro2[x][y]+=spectro[x+1][y+1]+2*spectro[x+1][y]+spectro[x+1][y-1];
				spectro2[x][y]/=16;
			}
			else// if(size == 2)
			{
				spectro2[x][y]=spectro[x-1][y];
				spectro2[x][y]+=1*spectro[x][y+1]+2*spectro[x][y]+1*spectro[x][y-1];
				spectro2[x][y]+=spectro[x+1][y];
				spectro2[x][y]/=6;

			}
		}
	}
	for(unsigned int y=0; y<spectro[0].size(); y++)
	{
		spectro2[0][y]=spectro2[1][y];
		spectro2[spectro2.size()-1][y]=spectro2[spectro2.size()-2][y];
	}
	return spectro2;
}
//*
//return -1 si pas de debut
int indice_debut(std::vector<std::vector<double> > spectro)
{
	std::vector<double> norme_bf, norme_hf;
	spectro=filtre_gaussien(spectro);
	for(std::vector<std::vector<double> >::iterator it = spectro.begin();
			it!=spectro.end()-1;it++)
	{
		norme_bf.push_back(sqrt(distance(*it,*(it+1), 0, it->size()/2)));
		norme_hf.push_back(sqrt(distance(*it,*(it+1),it->size()/2, it->size())));
		//norme.push_back(distance(*it));
	//	std::cout << norme_bf.back() << " " << norme_hf.back() << std::endl;
	}
	double max_hf = * std::max_element(norme_hf.begin(), norme_hf.end());
	double max_bf = * std::max_element(norme_bf.begin(), norme_bf.end());
	std::vector<unsigned int> indice_hf;
	std::vector<unsigned int> indice_bf;
	unsigned int nb_hf(0), nb_bf(0);
	for(unsigned int i=1; i<norme_hf.size(); i++)
	{
		if(norme_hf[i]>max_hf*2.0/3)
		{
			if(indice_hf.size()==0||indice_hf.back()!=i-nb_hf)
			{
				indice_hf.push_back(i);
				nb_hf=1;
			}
			else{
				nb_hf++;
			}
		}
		if(norme_bf[i]>max_bf*2.0/3){
			if(indice_bf.size()==0||indice_bf.back()!=i-nb_bf)
			{
				indice_bf.push_back(i);
				nb_bf=1;
			}
			else{
				nb_bf++;
			}
		}
	}
	
	if(indice_bf.size()==1&&indice_bf.back()<indice_hf.back())
		return indice_bf.back();
	//if(indice_hf.size()==1&&max_hf>max_bf)
	if(indice_hf.size()==1&&indice_bf.back()>indice_hf.back())
		return indice_hf.back();

	std::vector<std::pair<double,double> > energie_bf;
	for(unsigned int i : indice_bf){
		double e_tmp_inf(0);
		double e_tmp_sup(0);
		if(i>5&&i+10<spectro.size())
		{
			for(unsigned j = i-5;j<i; j++)
			{
				e_tmp_inf+=distance(spectro[j],0,spectro[j].size()/2);
			}
			for(unsigned j = i;j<i+10; j++)
			{
				e_tmp_sup+=distance(spectro[j],0,spectro[j].size()/2);
			}
		}
		energie_bf.push_back({e_tmp_inf/5,e_tmp_sup/10});
	}
	std::vector<std::pair<double,double> > energie_hf;
	for(unsigned int i : indice_hf){
		double e_tmp_inf(0);
		double e_tmp_sup(0);
		if(i>5&&i+10<spectro.size())
		{
			for(unsigned j = i-5;j<i; j++)
			{
				e_tmp_inf+=distance(spectro[j],
						spectro[j].size()/2,spectro[j].size());
			}
			for(unsigned j = i;j<i+10; j++)
			{
				e_tmp_sup+=distance(spectro[j],
						spectro[j].size()/2,spectro[j].size());
			}
		}
		energie_hf.push_back({e_tmp_inf/5,e_tmp_sup/10});
	}
	std::vector<unsigned int> indice_bf_2;
//	std::cout << "bf " << std::endl;
	for(unsigned int i =0; i<indice_bf.size(); i++)
	{
//		std::cout << indice_bf[i] <<" "<<energie_bf[i].first<<
//			" "<<energie_bf[i].second<<std::endl;
		if(energie_bf[i].first<energie_bf[i].second*4.0/5)
			indice_bf_2.push_back(indice_bf[i]);
	}
	std::vector<unsigned int> indice_hf_2;
//	std::cout << "hf " << std::endl;
	for(unsigned int i =0; i<indice_hf.size(); i++)
	{
//		std::cout << indice_hf[i] <<" "<<energie_hf[i].first<<
//			" "<<energie_hf[i].second<<std::endl;
		if(energie_hf[i].first<energie_hf[i].second*4.0/5)
			indice_hf_2.push_back(indice_hf[i]);
	}
//	std::cout << "1 return" << std::endl;
	if(indice_bf_2.size()==0)
	{
//		std::cout << "h" << indice_hf_2.size() << std::endl;
		if(indice_hf_2.size()==0)
			return -1;
		return indice_hf_2[0];
	}
	if(indice_hf_2.size()==0)
		return indice_bf_2[0];
//	std::cout << "2 return" << std::endl;
	if(indice_bf_2.size()==1&&indice_bf_2[0]<indice_hf_2[0])
		return indice_bf_2[0];
	if(indice_hf_2.size()==1&&indice_bf_2[0]>indice_hf_2[0])
		return indice_hf_2[0];
	
//	std::cout << "a finir " << __FILE__ << " " << __LINE__ << std::endl;
	
	if(indice_bf_2[0]<indice_hf_2[0])
		return indice_bf_2[0];
	return indice_hf_2[0];
	





/**
  todo
  */






	for(int i : indice_bf_2)
		std::cout << i << std::endl;
	std::cout << std::endl;
	for(int i : indice_hf_2)
		std::cout << i << std::endl;
	std::cout << std::endl;
	
	std::vector<unsigned int> bon_indice_bf;
//	std::vector<unsigned int> bon_indice_bf;
	bool good;
	for(unsigned int i : indice_bf){
		good=true;
		for(unsigned j = i;j<i+5&&j<spectro.size(); j++)
		{
			if(sqrt(distance(spectro[i-1],spectro[j],0,spectro[i].size()/2)<max_bf*2.0/3))
			{
				good=false;
				break;
			}
		}
		if(good)
		{
			bon_indice_bf.push_back(i);
		}
	}
	for(int i : bon_indice_bf)
		std::cout << i << std::endl;
	return bon_indice_bf[0];
	/*
	std::vector<std::pair<double,double> > energie_hf;
	for(unsigned int i : indice_hf){
		double e_tmp_inf(0);
		double e_tmp_sup(0);
		for(unsigned j = i<10?0:i-10;j<i; j++)
		{
			e_tmp_inf+=distance(spectro[j]);
		}
		for(unsigned j = i;j<i+10&&j<spectro.size(); j++)
		{
			e_tmp_sup+=distance(spectro[j]);
		}
		energie_hf.push_back({e_tmp_inf,e_tmp_sup});
	}
	std::cout << "bf" << std::endl;
	for(unsigned int i =0; i<indice_bf.size(); i++)
		std::cout << indice_bf[i] << " "
			<< energie_bf[i].first << " "
			<< energie_bf[i].second << std::endl;
	std::cout << "hf" << std::endl;
	for(unsigned int i =0; i<indice_hf.size(); i++)
		std::cout << indice_hf[i] << " "
			<< energie_hf[i].first << " "
			<< energie_hf[i].second << std::endl;
*/
	for(unsigned int i =0; i<indice_bf.size(); i++)
		std::cout << indice_bf[i] << std::endl;
	std::cout << "bon" << std::endl;
	for(unsigned int i =0; i<bon_indice_bf.size(); i++)
		std::cout << bon_indice_bf[i] << std::endl;
return -1;

/*	
	double max_val(norme[0]);
	unsigned int indice_1(0);
	unsigned int indice_2(0);
	unsigned int indice_3(0);
	//prendre 3 max _ val
	for(unsigned int i=1; i<norme.size(); i++)
	{
		if(norme[i]>max_val){
			max_val=norme[i];
			indice_3=indice_2;
			indice_2=indice_1;
			indice_1=i;
		}
	}
	return indice_1;
	if(indice_1<indice_3)
		return (indice_1<indice_2 ? indice_1 : indice_2);
	return (indice_2<indice_3 ? indice_2 : indice_3);
	/*std::vector<double> norme_2;
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
	//*/
}
//*/
struct surface_energie
{
	unsigned int indice_f;
	unsigned int indice_t;
	unsigned int delta_t;
	unsigned int delta_f_sur_2;
	double moyenne_surface;
};
std::vector<struct surface_energie> detection_haut_potentiel_energie(std::vector<std::vector<double> > spectro)
{
	std::vector<struct surface_energie> se;
	std::vector<double> max;
	spectro = filtre_gaussien(spectro);
	for(std::vector<double> spectre : spectro)
	{
		max.push_back(*std::max_element(spectre.begin(), spectre.end()));
	}

	return se;
}

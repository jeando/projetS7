#ifndef JOUEUR_H_INCLUDED
#define JOUEUR_H_INCLUDED

#include<iostream>
#include <vector>

class Joueur
{
    public:
        Joueur(int i, std::string nom);
        void charger();
		std::string id;
        std::string nom_joueur;
        std::vector<std::vector<double> > spectro_gauche1;
        std::vector<std::vector<double> > spectro_gauche2;
        std::vector<std::vector<double> > spectro_haut1;
        std::vector<std::vector<double> > spectro_haut2;
        std::vector<std::vector<double> > spectro_droite1;
        std::vector<std::vector<double> > spectro_droite2;
        std::vector<std::vector<double> > spectro_bas1;
        std::vector<std::vector<double> > spectro_bas2;
        std::vector<std::vector<double> > spectro_tir1;
        std::vector<std::vector<double> > spectro_tir2;

};

#endif // JOUEUR_H_INCLUDED

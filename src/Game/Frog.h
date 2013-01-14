#ifndef FROG_H_INCLUDED
#define FROG_H_INCLUDED

#include <string>

class Frog
{
    public:
        Frog(std::string nom);
        const std::string nom_image;

        int position_x;
        int position_y;
        int vitesse_x;
        int vitesse_y;
        int etat_x;
        int etat_y;
};

#endif // FROG_H_INCLUDED

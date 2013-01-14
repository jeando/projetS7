/**
* \details   Cette classe permet de décrit un objet dessinable
*/
#include "Drawable.h"

#include <iostream>
using namespace std;

/**
*constructeur de la classe drawable
*\param string img: le nom (l'adresse) de l'image à afficher lors du dessin de l'objet
**/
Drawable::Drawable(string img)
{
    const char* image = img.c_str();
    my_image = IMG_Load( image );
}

/**
*fonction permetant de changer l'image a afficher
*\param string img: le nouveau nom (adresse) de l'image
*/
void Drawable::set_image(string img)
{
    const char* image = img.c_str();
    my_image = IMG_Load( image );
}

/**
*destructeur de la classe
*/
Drawable::~Drawable()
{
    SDL_FreeSurface(my_image);
}

/**
*maniere standard d'afficher une image
*\param SDL_Surface* screen: l'ecran sur lequel sera dessine l'image
*/
void Drawable::draw(SDL_Surface* screen)
{
    SDL_BlitSurface(my_image,NULL,screen,NULL);//image à prendre, toute l'image à partir de l'origine, sur quoi on colle, origine
}

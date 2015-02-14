/* 
 * File:   Element_portable.h
 * Author: Azarias
 *
 * Created on 3 février 2015, 09:53
 */

#ifndef ELEMENT_PORTABLE_H
#define	ELEMENT_PORTABLE_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <typeinfo>

class Joueur;


using namespace sf;
using namespace std;

class Element_portable {
public:
    Element_portable();
    Element_portable(Sprite image,int t_elem);
    void setSprite(Sprite image);
    void setType(int newtype);
    inline int getType(){ return id_elem;};
    Sprite& getSprite();
    Vector2f getPosition();
    void changePorteState(bool nouvelEtat);
    void changerPorteur(Joueur* porteur);
    void moveCarried();
    inline bool isCarried(){return estPorte;};
    inline virtual ~Element_portable() {
    };
protected:
    virtual void setTexture(Texture maTexture);
    bool estPorte = false;
    Sprite pSprite;
    Joueur* pporteur;
    Texture pTexture;
    int id_elem;

};

#endif	/* ELEMENT_PORTABLE_H */


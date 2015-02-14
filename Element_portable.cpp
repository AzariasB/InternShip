/* 
 * File:   Element_portable.cpp
 * Author: Azarias
 * 
 * Created on 3 février 2015, 09:53
 */

#include "Element_portable.hpp"
#include "Joueur.hpp"
#include "Game.hpp"

Element_portable::Element_portable() {
}

Element_portable::Element_portable(Sprite image, int t_elem)
:pSprite(image),
id_elem(t_elem){

}

void Element_portable::setType(int newtype) {
    id_elem = newtype;
}


sf::Sprite& Element_portable::getSprite() {
    return pSprite;
}

void Element_portable::setSprite(sf::Sprite image) {
    pSprite = image;
}

void Element_portable::changePorteState(bool nouvelEtat) {
    estPorte = nouvelEtat;
}

void Element_portable::setTexture(sf::Texture maTexture) {
    pTexture = maTexture;
}

void Element_portable::changerPorteur(Joueur* porteur) {
    pporteur = porteur;
}

void Element_portable::moveCarried() {
    if (pporteur != nullptr) {
        Vector2f s_ppos = pporteur->getSprite().getPosition();
        pSprite.setPosition(s_ppos.x, s_ppos.y - 32);
        if (id_elem == JOUEUR ) {
            //Changer direction du joueur porté
            (static_cast<Joueur*>(this))->changeDirection(pporteur->getDirection());
            (static_cast<Joueur*>(this))->walk_animation();
        }
    }
}

Vector2f Element_portable::getPosition() {
    return pSprite.getPosition();
}

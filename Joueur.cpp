/* 
 * File:   Joueur.cpp
 * Author: Mountrip
 * 
 * Created on 3 février 2015, 09:53
 */

#include "Joueur.hpp"

Joueur::Joueur()
: cequejeporte(nullptr) {
    set_anim(1, DOWN);
    pwalk = false;
}

Joueur::Joueur(sf::Texture playerTexture)
: cequejeporte(nullptr) {
    set_anim(1, DOWN);
    pTexture = playerTexture;
    pSprite.setTexture(pTexture);
    set_anim(1, 0);
    pSprite.setTextureRect(sf::IntRect(panim.x*P_SIZE, panim.y*P_SIZE, P_SIZE, P_SIZE));
    set_height(pSprite.getGlobalBounds().height);
    set_width(pSprite.getGlobalBounds().width);
    pwalk = false;
    Element_portable::setSprite(pSprite);
}

void Joueur::set_height(int height) {
    p_height = height;
}

void Joueur::set_width(int witdth) {
    p_width = witdth;
}

void Joueur::changeDirection(int direction) {
    pdirection = direction;
    change_y_animation(pdirection);
}

void Joueur::set_anim(int x_anim, int y_anim) {
    panim.x = x_anim;
    panim.y = y_anim;
}

void Joueur::setTexture(sf::Texture newTexture) {
    pTexture = newTexture;
    pSprite.setTexture(pTexture);
    pSprite.setTextureRect(sf::IntRect(panim.x*P_SIZE, panim.y*P_SIZE, P_SIZE, P_SIZE));
    set_height(pSprite.getGlobalBounds().height);
    set_width(pSprite.getGlobalBounds().width);
    Element_portable::setSprite(pSprite);
}

void Joueur::changeWalkState(bool walk) {
    pwalk = walk;
}

void Joueur::walk_animation() {
    if (!estPorte) {
        panim.x++;
    }
    if (panim.x * 32 >= pTexture.getSize().x) {
        panim.x = 0;
    }
    pSprite.setTextureRect(sf::IntRect(panim.x * P_SIZE, panim.y * P_SIZE, P_SIZE, P_SIZE));

}

void Joueur::change_y_animation(int newy) {
    if (pporte) {
        panim.y = newy + 4;
    } else {
        panim.y = newy;
    }
}

void Joueur::changeCarryState(bool carry) {
    pporte = carry;
}

void Joueur::is_Carrying(Element_portable& porte) {
    pporte = true;
    //Joueur moi = this;
    porte.changerPorteur(this);
    porte.changePorteState(true);
    porte.getSprite().setPosition(pSprite.getPosition().x, pSprite.getPosition().y - 32);
    cequejeporte = &porte;
}

void Joueur::tp_vers(Joueur joueur) {
    getSprite().setPosition(joueur.getSprite().getPosition().x, joueur.getSprite().getPosition().y);
}

bool Joueur::canCarry() {
    return !pwalk && !seTp && !pporte && !estPorte && cequejeporte == nullptr && !seNoie && !saute;
}

bool Joueur::canTp() {
    return !pwalk && !seTp && !pporte && !estPorte && cequejeporte == nullptr && !seNoie && !saute;
}

bool Joueur::canWalk() {
    return !seTp && !estPorte && !seNoie && !saute;
}

void Joueur::lay_down() {
    pporte = false;
    cequejeporte = nullptr;
}

Element_portable* Joueur::cequetuPortes() {
    return cequejeporte;
}

void Joueur::noyade_animation() {
    panim.y = getDirection() + 14;
    panim.x = etat_noyade;
    pSprite.setTextureRect(sf::IntRect(panim.x * P_SIZE, panim.y * P_SIZE, P_SIZE, P_SIZE));
    etat_noyade++;
    if (etat_noyade > 2) {
        //Réinitialisation de la noyade
        etat_noyade = -1;
        seNoie = false;
        panim.y = getDirection();
        pSprite.setTextureRect(sf::IntRect(panim.x * P_SIZE, panim.y * P_SIZE, P_SIZE, P_SIZE));
        pSprite.setPosition(destination_noy);
        destination_noy = Vector2f(0.f, 0.f);
    }
}

void Joueur::tp_animation() {
    panim.y = 12;
    panim.x = etat_tp % 3;
    if (etat_tp > 2) {
        panim.y = 13;
    }
    pSprite.setTextureRect(sf::IntRect(panim.x * P_SIZE, panim.y * P_SIZE, P_SIZE, P_SIZE));
    if (etat_tp == 2) {
        pSprite.setPosition(destination_tp);
    }
    etat_tp++;
    if (etat_tp > 6) {
        //Fin de la tp
        etat_tp = -1;
        seTp = false;
        panim.y = getDirection();
        destination_tp = Vector2f(0.f, 0.f);
        pSprite.setTextureRect(sf::IntRect(panim.x * P_SIZE, panim.y * P_SIZE, P_SIZE, P_SIZE));
    }

}

void Joueur::jump_animation() {
    panim.y = getDirection() + 8;
    panim.x = etat_saut;
    pSprite.setTextureRect(sf::IntRect(panim.x * P_SIZE, panim.y * P_SIZE, P_SIZE, P_SIZE));
    Vector2f deplacement = (getPosition() + destination_saut) / 2.f;
    pSprite.setPosition(deplacement);
    etat_saut++;
    if (etat_saut > 2) {
        //Réinitialisation du saut
        etat_saut = -1;
        saute = false;
        panim.y = getDirection();
        destination_saut = Vector2f(0.f, 0.f);
        pSprite.setTextureRect(sf::IntRect(panim.x * P_SIZE, panim.y * P_SIZE, P_SIZE, P_SIZE));
    }

}

void Joueur::activateJump(Vector2f destination) {
    destination_saut = destination;
    etat_saut = 0;
    saute = true;
}

void Joueur::activateTp(Vector2f destination) {
    destination_tp = destination;
    etat_tp = 0;
    seTp = true;
}

void Joueur::activateSink(Vector2f destination) {
    destination_noy = destination;
    etat_noyade = 0;
    seNoie = true;
}

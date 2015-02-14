/* 
 * File:   Joueur.h
 * Author: Azarias
 *
 * Created on 3 février 2015, 09:53
 */

#ifndef JOUEUR_H
#define	JOUEUR_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>
#include <typeinfo>

#include "Element_portable.hpp"
class Joueur;

#define P_SIZE 32
#define P_SPEED 64

enum direction {
    DOWN, LEFT, RIGHT, UP, TOTAL_DIRECTIONS
};

using namespace std;
using namespace sf;

class Joueur : public Element_portable {
public:
    Joueur();
    Joueur(Texture playerTexture);
    void changeDirection(int direction);
    void changeWalkState(bool walk);
    void is_Carrying(Element_portable& porte);
    void lay_down();
    void setTexture(Texture newTexture);
    void change_y_animation(int newy);
    void walk_animation();
    void jump_animation();
    void noyade_animation();
    void tp_animation();
    void tp_vers(Joueur joueur);
    void activateJump(Vector2f destination);
    void activateTp(Vector2f destination);
    void activateSink(Vector2f destination);
    int getDirection(){return pdirection; } ;
    bool isWalking(){return pwalk;};
    bool isSinking(){return seNoie;};
    bool isJumping(){return saute;};
    bool isCarrying(){return pporte;};
    bool isTping(){return seTp;};
    bool canTp();
    bool canCarry();
    bool canWalk();
    Element_portable* cequetuPortes();

    virtual ~Joueur() {
    };
    //    friend bool operator==(Joueur& j1, Joueur& j2);
private:
    void set_height(int height);
    void set_width(int witdth);
    void set_anim(int x, int y);
    void changeCarryState(bool carry);
    

    Vector2i panim;

    bool pwalk = false;
    bool pporte = false;
    
    bool seTp = false;
    int etat_tp = -1;
    Vector2f destination_tp;
    
    bool saute = false;
    int etat_saut = -1;
    Vector2f destination_saut;
    
    bool seNoie = false;
    int etat_noyade = -1;
    Vector2f destination_noy;
    
    
    
    Element_portable* cequejeporte;
    
    int pdirection = DOWN;
    int p_height;
    int p_width;

};

#endif	/* JOUEUR_H */


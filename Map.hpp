/* 
 * File:   Map.h
 * Author: Azarias
 *
 * Created on 4 février 2015, 17:52
 */

#ifndef MAP_H
#define	MAP_H

#include <iostream>
#include <istream>
#include <vector>
#include <string>

#include "Joueur.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#define TILE_SIZE 16

using namespace std;
using namespace sf;

enum FILTRES {
    FOND, QUITUE, DECORATION, COLLISION, DECO_COLLISION, COLLISION_PASSABLE, SOMMET_COLLISION
};


//Structure poru les liens d'une map à l'autre

struct map_link {
    Vector2i posit_l;
    int num_map;
};

//Structure pour les trigger d'une map

struct map_trigger {
    Vector2i posit_trigger;
    Vector2i posit_trigged;
    int id_tile_ajoute;
    int id_tile_enleve;
    int filtreAjoute;
    int filtreEnleve;
    int id_objet;
    int temporalite;
};


class Map {
public:
    Map();
    Map(string nomCarte);
    void load(string nomCarte);
    void drawBg(RenderWindow& fenetre);
    void drawFg(RenderWindow& fenetre);
    void setMap(string nomCarte);
    void setSpawnPoint(Vector2f point);
    int getLink(Joueur joueur);
    bool HitDecoBlock(Sprite element);
    bool HitBlock(Sprite element);
    bool HitPassableBlock(Sprite element);
    bool FallandDie(Sprite element);
    void ajouterElement(Element_portable* nouvelelemnt, int t_elem);
    void lancerJoueur(Joueur& j_lanceur, Element_portable& elem_lance);
    void triggerMap(Joueur& jouer_t);
    Vector2i getMapSize();
    Element_portable* find_elemport(Joueur porteur, int direction);
    vector<Element_portable*> getElements();
    Vector2i getPositInArray(Sprite element);
    Vector2f getSpawnPoint();

    virtual inline ~Map() {
    };
private:
    void setCarte(vector<vector<vector<int>>> tableau);
    void initialiseTexture();
    void initialiseLink(string nomFichier);
    void initialiseTrigger(string nomFichier);
    bool sameSprite(Sprite& first, Sprite& seconde);
    void executeTrigger(map_trigger instructions, Joueur& declencheur);
    inline vector<vector<vector<Sprite>>>* getSprites(){return &mesSprites;};
    Sprite getSpriteFromIndex(int index);

    vector<Element_portable*> mesElements;
    vector<vector<vector<Sprite>>> mesSprites;
    vector<vector<vector<int>>> maCarte;


    //Les différents éléments de la map
    vector<map_link> links;
    vector<map_trigger> triggers;
    vector<Texture> objet_texture;

    Vector2f spawnPoint;


};

#endif	/* MAP_H */


/* 
 * File:   Map.cpp
 * Author: Azarias
 * 
 * Created on 4 février 2015, 17:52
 */

#include "Map.hpp"
#include "ReadFile.hpp"
#include "Game.hpp"
#include "Element_portable.hpp"
#include "Joueur.hpp"

Map::Map() {
    ;
}

Map::Map(string nomCarte) {
    setMap(nomCarte);
    initialiseLink(nomCarte);
    initialiseTrigger(nomCarte);
}

void Map::load(string nomCarte) {
    setMap(nomCarte);
    initialiseLink(nomCarte);
    initialiseTrigger(nomCarte);
}

void Map::drawBg(RenderWindow& fenetre) {
    for (int tab = 0; tab < mesSprites.size(); tab++) {
        vector<vector < Sprite>> tableau = mesSprites[tab];
        for (int ligne = 0; ligne < tableau.size(); ligne++) {
            if (tab != SOMMET_COLLISION) {
                vector< Sprite> sprite_c = tableau[ligne];
                for (int colonne = 0; colonne < sprite_c.size(); colonne++) {
                    fenetre.draw(sprite_c[colonne]);
                }
            }
        }
    }
}

void Map::drawFg(RenderWindow& fenetre) {
    for (int y = 0; y < mesSprites[SOMMET_COLLISION].size(); y++) {
        for (int x = 0; x < mesSprites[SOMMET_COLLISION][y].size(); x++) {
            fenetre.draw(mesSprites[SOMMET_COLLISION][y][x]);
        }
    }
}

vector<Element_portable*> Map::getElements() {
    return mesElements;
}

Vector2f Map::getSpawnPoint() {
    return spawnPoint;
}

void Map::setMap(string nomCarte) {
    ReadFile f(nomCarte.c_str());
    setCarte(f.fileToArray());
    initialiseTexture();
}

void Map::setCarte(vector<vector<vector<int> > > tableau) {
    maCarte = tableau;
}

void Map::initialiseTexture() {
    for (int tableau = 0; tableau < maCarte.size(); tableau++) {
        vector<vector< Sprite> > SpriteTableau;
        vector<vector<int> >& untableau = maCarte[tableau];
        for (int ligne = 0; ligne < untableau.size(); ligne++) {
            vector< Sprite> spriteLigne;
            for (int chiffre = 0; chiffre < untableau[ligne].size(); chiffre++) {
                int sprite_pos = untableau[ligne][chiffre];
                Sprite tmpSp;
                if (sprite_pos != 0) {
                    tmpSp = getSpriteFromIndex(sprite_pos);
                    tmpSp.setPosition(chiffre*TILE_SIZE, ligne * TILE_SIZE);
                    spriteLigne.push_back(tmpSp);
                } else {
                    Sprite sp;
                    sp.setTexture(Game::transparent);
                    sp.setPosition(chiffre*TILE_SIZE, ligne * TILE_SIZE);
                    spriteLigne.push_back(sp);
                }

            }
            SpriteTableau.push_back(spriteLigne);
        }
        mesSprites.push_back(SpriteTableau);
    }
}

/*
 Toutes les fonctions de collision
 */
bool Map::HitDecoBlock(Sprite element) {
    Vector2i tab_coo = getPositInArray(element);
    return maCarte[DECO_COLLISION][tab_coo.y][tab_coo.x] != 0;
}

bool Map::HitBlock(Sprite element) {
    Vector2i tab_coo = getPositInArray(element);
    return maCarte[COLLISION][tab_coo.y][tab_coo.x] != 0;
}

bool Map::FallandDie(Sprite element) {
    Vector2i coord = getPositInArray(element);
    return maCarte[QUITUE][coord.y][coord.x] != 0;
}

bool Map::HitPassableBlock(Sprite element) {
    Vector2i coord = getPositInArray(element);
    return maCarte[COLLISION_PASSABLE][coord.y][coord.x] != 0;
}

/*
 C'est cette fonction qui permet de calculer la HitBox quelque soit le bloc
 */
Vector2i Map::getPositInArray(Sprite element) {
    Vector2i coordone_tableau;
    coordone_tableau.x = (int) ((element.getPosition().x + P_SIZE / 2) / (float) TILE_SIZE);
    coordone_tableau.y = (int) ((element.getPosition().y + P_SIZE) / (float) TILE_SIZE);

    return coordone_tableau;
}

void Map::setSpawnPoint(Vector2f point) {
    this->spawnPoint = point;
}

int Map::getLink(Joueur joueur) {
    Sprite j_pos = joueur.getSprite();
    Vector2i coord = getPositInArray(j_pos);
    for (int i = 0; i < links.size(); i++) {
        //cout << "Coord joueur : x =" << coord.x << " y =" << coord.y << " == Coord à être : x=" << links[i].posit_l.x << "-y=" << links[i].posit_l.y << endl;
        if (links[i].posit_l == coord) {
            return links[i].num_map;
        }
    }
    return -1;
}

void Map::initialiseLink(string nomFichier) {
    int map_num;
    Vector2i coord;
    string nomlink = nomFichier + ".link";
    ifstream links_f;
    links_f.open(nomlink);
    int unentier;
    int i = 0;
    int iteration = 0;
    if (links_f.is_open()) {
        while (links_f >> unentier) {
            if (i == 0) {
                coord.x = unentier;
                i++;
            } else if (i == 1) {
                coord.y = unentier;
                i++;
            } else if (i == 2) {
                map_num = unentier;
                links.push_back(map_link());
                links[iteration].posit_l = coord;
                links[iteration].num_map = map_num;
                iteration++;
                i = 0;
            }
        }
    } else {
        cout << "Impossible d'ouvrir le fichier : " << nomlink << endl;
    }
}

void Map::ajouterElement(Element_portable* nouvelelemnt, int elem_type) {
    nouvelelemnt->setType(elem_type);
    mesElements.push_back(nouvelelemnt);
}

Element_portable* Map::find_elemport(Joueur porteur, int direction) {
    int x_pos = 0;
    int y_pos = 0;
    switch (direction) {
        case UP:
            y_pos = -1;
            break;
        case DOWN:
            y_pos = 1;
            break;
        case LEFT:
            x_pos = -1;
            break;
        case RIGHT:
            x_pos = 1;
            break;
    }
    Vector2i posit_joueur = getPositInArray(porteur.getSprite());
    for (int i = 0; i < mesElements.size(); i++) {
        Vector2i elem_list = getPositInArray(mesElements[i]->getSprite());
        //cout << " Joueur :  x=" << posit_joueur.x << "-y = " << posit_joueur.y << " Element de la liste : x " << elem_list.x << "-y =" << elem_list.y << endl;
        if (((elem_list.x == posit_joueur.x && elem_list.y == posit_joueur.y)
                || (elem_list.x == posit_joueur.x + x_pos && elem_list.y == posit_joueur.y + y_pos)
                || (elem_list.x == posit_joueur.x + x_pos + y_pos && elem_list.y == posit_joueur.y + y_pos + x_pos)
                || (elem_list.x == posit_joueur.x + x_pos - y_pos && elem_list.y == posit_joueur.y + y_pos - x_pos))
                && !sameSprite(porteur.getSprite(), mesElements[i]->getSprite())) {
            return mesElements[i];
        }
    }
    return nullptr;
}

bool Map::sameSprite(Sprite& first, Sprite& seconde) {
    return first.getGlobalBounds().height == seconde.getGlobalBounds().height
            && first.getTexture() == seconde.getTexture()
            && first.getPosition() == seconde.getPosition();
}

void Map::lancerJoueur(Joueur& j_lanceur, Element_portable& elem_lance) {
    j_lanceur.lay_down();
    elem_lance.changePorteState(false);
    elem_lance.changerPorteur(nullptr);
    Element_portable sans_reference = elem_lance;

    Vector2i lanceur_pos = getPositInArray(j_lanceur.getSprite());

    int direct = j_lanceur.getDirection();
    int x_incr = 0;
    int y_incr = 0;

    switch (direct) {
        case UP:
            y_incr = -1;
            break;
        case DOWN:
            y_incr = 1;
            break;
        case LEFT:
            x_incr = -1;
            break;
        case RIGHT:
            x_incr = 1;
            break;
    }

    //De base, on met le joueur à cette position
    sans_reference.getSprite().setPosition(j_lanceur.getPosition());
    Sprite elem_pos = elem_lance.getSprite();

    for (int case_par = 0; case_par < 4; case_par++) {
        //cout << " On incrémente donc : x=" << x_incr << "-y=" << y_incr << endl;
        lanceur_pos.x += x_incr;
        lanceur_pos.y += y_incr;
        elem_pos.move((float) (TILE_SIZE * x_incr), (float) (TILE_SIZE * y_incr));
        if (!HitBlock(elem_pos) || (case_par == 2 && !HitPassableBlock(elem_pos))) {
            sans_reference.getSprite().setPosition(elem_pos.getPosition());
        } else {
            elem_pos.move((float) (TILE_SIZE*-x_incr), (float) (TILE_SIZE*-y_incr));
            break;
        }
    }
    (static_cast<Joueur*>(&elem_lance))->activateJump(sans_reference.getPosition());
}

void Map::initialiseTrigger(string nomFichier) {
    Vector2i coordButton;
    Vector2i coordTrigged;
    int id_tile_ajoute;
    int id_tile_enleve;
    int filtreEnleve;
    int filtreAjoute;
    int idObjet;
    int temp;
    string nomTrigger = nomFichier + ".trigger";
    ifstream triggers_f;
    triggers_f.open(nomTrigger);
    int unentier;
    int i = 0;
    int iteration = 0;
    if (triggers_f.is_open()) {
        while (triggers_f >> unentier) {
            if (i == 0) {
                coordButton.x = unentier;
                i++;
            } else if (i == 1) {
                coordButton.y = unentier;
                i++;
            } else if (i == 2) {
                coordTrigged.x = unentier;
                i++;
            } else if (i == 3) {
                coordTrigged.y = unentier;
                i++;
            } else if (i == 4) {
                id_tile_ajoute = unentier;
                i++;
            } else if (i == 5) {
                id_tile_enleve = unentier;
                i++;
            } else if (i == 6) {
                filtreAjoute = unentier;
                i++;
            } else if (i == 7) {
                filtreEnleve = unentier;
                i++;
            } else if (i == 8) {
                idObjet = unentier;
                i++;
            } else if (i == 9) {
                temp = unentier;
                triggers.push_back(map_trigger());
                triggers[iteration].posit_trigger = coordButton;
                triggers[iteration].posit_trigged = coordTrigged;
                triggers[iteration].id_tile_ajoute = id_tile_ajoute;
                triggers[iteration].id_tile_enleve = id_tile_enleve;
                triggers[iteration].filtreAjoute = filtreAjoute;
                triggers[iteration].filtreEnleve = filtreEnleve;
                i = 0;
                iteration++;

            }
        }
    } else {
        cout << "Impossible d'ouvrir le fichier : " << nomTrigger << endl;
    }
}

void Map::triggerMap(Joueur& joueur_t) {
    Vector2i posit_j = getPositInArray(joueur_t.getSprite());
    for (int trigger = 0; trigger < triggers.size(); trigger++) {
        if (triggers[trigger].posit_trigger == posit_j) {
            executeTrigger(triggers[trigger], joueur_t);
        }
    }

}

void Map::executeTrigger(map_trigger instructions, Joueur& declencheur) {
    Vector2i posit_j = getPositInArray(declencheur.getSprite());
    int x_trigd = instructions.posit_trigged.x;
    int y_tridg = instructions.posit_trigged.y;
    int f_Aj = instructions.filtreAjoute;
    int f_En = instructions.filtreEnleve;

    Sprite & sprite_aj = mesSprites[f_Aj][y_tridg][x_trigd];
    Sprite & sprite_en = mesSprites[f_En][y_tridg][x_trigd];
    int& carte_aj = maCarte[f_Aj][y_tridg][x_trigd];
    int& carte_en = maCarte[f_En][y_tridg][x_trigd];

    Sprite nouveauSprite;
    Sprite transparent;
    transparent.setTexture(Game::transparent);

    nouveauSprite = getSpriteFromIndex(instructions.id_tile_ajoute);
    sprite_aj = transparent;
    sprite_en = nouveauSprite;
    carte_aj = instructions.id_tile_ajoute;
    carte_en = 0;
}

Sprite Map::getSpriteFromIndex(int index) {
    int x_pos = (index % 64) - 1;
    int y_pos = ((index - x_pos) / 64);
    Sprite tmpSp;
    tmpSp.setTexture(Game::tileSet);
    tmpSp.setTextureRect(IntRect(x_pos*TILE_SIZE, y_pos*TILE_SIZE, TILE_SIZE, TILE_SIZE));
    return tmpSp;
}

Vector2i Map::getMapSize() {
    return Vector2i(maCarte[0][0].size(),maCarte[0].size());
}

/* 
 * File:   Game.h
 * Author: Azarias
 *
 * Created on 5 novembre 2014, 19:30
 */

#ifndef GAME_H
#define	GAME_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "Joueur.hpp"
#include "Map.hpp"

#define TILE_SIZE 16
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS  seconds(1.f/60.f)

enum MAP{TUTO,PRINCIPALE,M_1,M_2,GROTTE,M_FINALE};
enum IDOBJET{RIEN,HACHE,CLEF,PIERRE,CV,Motiv,JOUEUR,TOTAL_OBJETS};

using namespace std;
using namespace sf;

class Game {
public:
    Game();
    ~Game() {};
    void run();
    
    static Texture tileSet;
    static Texture transparent;
private:
    void render();
    void processEvents();
    void renderMap();
    void renderPlayer();
    void update(Time deltaTime);
    void handlePlayerInput(Keyboard::Key key, bool release);
    void chargementTexture();
    void bougerJoueur(Joueur& joueur, Time deltaTime);
    void setView();
    void renderForeGround();
    void chargerCartes();
    void loadTileSet();
    void changerMap(int numcarte,Sprite& newSpawnPoint);
    bool outOfview(Sprite& positSprite);
    void loading();
    void changerLoading(string texte,Text& achanger,Sprite& fond,RenderWindow& fenetre);
    vector<vector<int>> getSpawnPoints();
    vector<int> traiteString(string atraiter);
    Map& getCurrentMap();


    RenderWindow mWindow;
    
    Clock gameTime;
    View vue_jeu;
//    Music musique;

    Joueur joueur1;
    Joueur joueur2;
    int current_map = 0;
    vector<Map> mesMaps;
};

#endif	/* GAME_H */


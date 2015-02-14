/* 
 * File:   Game.cpp
 * Author: Azarias
 * 
 * Created on 5 novembre 2014, 19:30
 */

#include "Game.hpp"

Texture Game::tileSet;
Texture Game::transparent;

Game::Game()
: mWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Internship") {
    loading();
}

void Game::run() {
    setView();
    mWindow.setMouseCursorVisible(false);
    vue_jeu.zoom(0.6);
    Clock clock;
    Time lastUpdate = Time::Zero;
    while (mWindow.isOpen()) {
        processEvents();
        lastUpdate += clock.restart();
        while (lastUpdate > FPS) {
            lastUpdate -= FPS;
            processEvents();
            update(FPS);
        }
        render();
    }
}

void Game::processEvents() {
    Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == Event::Closed) {
            mWindow.close();
        }
        switch (event.type) {
            case Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;
            case Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;
            case Event::Closed:
                mWindow.close();
                break;

        }
    }
}

void Game::render() {
    mWindow.clear();
    mWindow.setView(vue_jeu);
    renderMap();
    renderPlayer();
    renderForeGround();
    mWindow.display();
}

void Game::renderForeGround() {
    //Dessiner le fg de la carte courante
    mesMaps[current_map].drawFg(mWindow);
}

void Game::renderMap() {
    //Dessiner la carte courante
    mesMaps[current_map].drawBg(mWindow);
}

void Game::handlePlayerInput(Keyboard::Key key, bool isPressed) {
    bool new_input_j1 = false;
    bool new_input_j2 = false;
    Keyboard clavier;
    if (isPressed) {
        // Contrôles du joueur 1
        if (key == clavier.Q) {
            joueur1.changeDirection(LEFT);
            new_input_j1 = true;
        }
        if (key == clavier.D) {
            joueur1.changeDirection(RIGHT);
            new_input_j1 = true;
        }
        if (key == clavier.Z) {
            joueur1.changeDirection(UP);
            new_input_j1 = true;
        }
        if (key == clavier.S) {
            joueur1.changeDirection(DOWN);
            new_input_j1 = true;
        }
        if (key == clavier.E && joueur1.canTp()) {
            joueur1.activateTp(joueur2.getPosition());
        }
        if (key == clavier.A && joueur1.canCarry()) {
            Element_portable* porte = getCurrentMap().find_elemport(joueur1, joueur1.getDirection());
            if (porte != nullptr) {
                joueur1.is_Carrying(*porte);
            }
        } else if (key == clavier.A && joueur1.isCarrying()) {
            getCurrentMap().lancerJoueur(joueur1, joueur2);
        }

        //Fin des contrôles du joueur1

        //Contrôles du joueur 2
        if (key == clavier.Left) {
            joueur2.changeDirection(LEFT);
            new_input_j2 = true;
        }
        if (key == clavier.Right) {
            joueur2.changeDirection(RIGHT);
            new_input_j2 = true;
        }
        if (key == clavier.Up) {
            joueur2.changeDirection(UP);
            new_input_j2 = true;
        }
        if (key == clavier.Down) {
            joueur2.changeDirection(DOWN);
            new_input_j2 = true;
        }
        if (key == clavier.RShift && joueur2.canTp()) {
            joueur2.activateTp(joueur1.getPosition());
        }
        if (key == clavier.RControl && joueur2.canCarry()) {
            Element_portable* porte = getCurrentMap().find_elemport(joueur2, joueur2.getDirection());
            if (porte != nullptr) {
                joueur2.is_Carrying(*porte);
            }
        } else if (key == clavier.RControl && joueur2.isCarrying()) {
            getCurrentMap().lancerJoueur(joueur2, joueur1);
        }

        //Fin des contrôles du joueur2

        //Savoir lequel bouger
        if (new_input_j1 && joueur1.canWalk()) {
            joueur1.changeWalkState(true);
        }
        if (new_input_j2 && joueur2.canWalk()) {
            joueur2.changeWalkState(true);
        }
    } else {
        if (key == clavier.Down || key == clavier.Up || key == clavier.Left || key == clavier.Right) {
            joueur2.changeWalkState(false);
        }
        if (key == clavier.Q || key == clavier.D || key == clavier.S || key == clavier.Z) {
            joueur1.changeWalkState(false);
        }
    }



}

void Game::update(Time deltaTime) {
    if (joueur1.isWalking() || joueur2.isWalking() || joueur1.isJumping() || joueur2.isJumping() || joueur1.isTping() || joueur2.isTping() || joueur1.isSinking() || joueur2.isSinking()) {
        if (gameTime.getElapsedTime().asMilliseconds() >= 75) {
            if (joueur1.isWalking()) {
                bougerJoueur(joueur1, deltaTime);
                if (joueur2.isCarried()) {
                    joueur2.moveCarried();
                }
            }
            if (joueur2.isWalking()) {
                bougerJoueur(joueur2, deltaTime);
                if (joueur1.isCarried()) {
                    joueur1.moveCarried();
                }
            }
            if (joueur1.isJumping()) {
                joueur1.jump_animation();
            }
            if (joueur2.isJumping()) {
                joueur2.jump_animation();
            }
            if (joueur1.isTping()) {
                joueur1.tp_animation();
            }
            if (joueur2.isTping()) {
                joueur2.tp_animation();
            }
            if (joueur1.isSinking()) {
                joueur1.noyade_animation();
            }
            if (joueur2.isSinking()) {
                joueur2.noyade_animation();
            }
            gameTime.restart();
        }
    }
}

void Game::renderPlayer() {
    mWindow.draw(joueur1.getSprite());
    mWindow.draw(joueur2.getSprite());
}

void Game::chargementTexture() {
    Texture textp1;
    Texture texpt2;
    if (!textp1.loadFromFile("joueur1.png") || !texpt2.loadFromFile("joueur2.png")) {
        cout << "Erreur lors du chargement des textures des joueurs" << endl;
        exit(EXIT_FAILURE);
    }
    joueur1.setTexture(textp1);
    joueur2.setTexture(texpt2);
}

void Game::setView() {
    
    Vector2f positJ1 = joueur1.getPosition();
    Vector2f positJ2 = joueur2.getPosition();
    Vector2i mapSize =  getCurrentMap().getMapSize();
    Vector2f positView = (positJ1 / 2.f + positJ2 / 2.f);
    
    vue_jeu.setCenter(positView);
    mWindow.setView(vue_jeu);
}

void Game::bougerJoueur(Joueur& joueur, Time deltaTime) {
    joueur.walk_animation();
    Vector2f deplacement_j1(0.f, 0.f);
    joueur.change_y_animation(joueur.getDirection());
    switch (joueur.getDirection()) {
        case UP:
            deplacement_j1.y += -P_SPEED*TILE_SIZE;
            break;
        case DOWN:
            deplacement_j1.y += P_SPEED*TILE_SIZE;
            break;
        case LEFT:
            deplacement_j1.x += -P_SPEED*TILE_SIZE;
            break;
        case RIGHT:
            deplacement_j1.x += P_SPEED*TILE_SIZE;
            break;
    }
    joueur.getSprite().move(deplacement_j1 * deltaTime.asSeconds());
    if (getCurrentMap().HitBlock(joueur.getSprite()) || getCurrentMap().HitPassableBlock(joueur.getSprite()) || getCurrentMap().HitDecoBlock(joueur.getSprite())) {
        joueur.getSprite().move(-(deplacement_j1 * deltaTime.asSeconds()));
    } else {
        setView();
    }
    int ch_map = getCurrentMap().getLink(joueur);
    if (ch_map != -1) {
        joueur.getSprite().move(-(deplacement_j1 * deltaTime.asSeconds()));
        changerMap(ch_map, joueur.getSprite());
    }
    if (outOfview(joueur.getSprite())) {
        joueur.getSprite().move(-(deplacement_j1 * deltaTime.asSeconds()));
    }
    if (getCurrentMap().FallandDie(joueur.getSprite())) {
        joueur.activateSink(getCurrentMap().getSpawnPoint());
    }
    getCurrentMap().triggerMap(joueur);
}

void Game::loadTileSet() {
    if (!Game::tileSet.loadFromFile("tiles.png") || !Game::transparent.loadFromFile("tiles.png", IntRect(0, 0, 16, 16))) {
        cout << "Impossible de charger les tuiles" << endl;
        exit(EXIT_FAILURE);
    }
}

void Game::chargerCartes() {
    vector<vector<int>> spawnpoints = getSpawnPoints();
    vector<string> nomcartes = {"tuto.map", "principale.map", "maisonCV.map", "maisonHache.map", "grotte.map", "maisonFin.map"};
    //vector<string> nomcartes = {"principale.map","grotte.map" ,"maisonFin.map"};//, "maisonCV.map", "maisonHache.map", "grotte.map", "maisonFin.map"};
    for (int i = 0; i < nomcartes.size(); i++) {
        Map map;
        mesMaps.push_back(map);
        mesMaps[i].load("Maps/" + nomcartes[i]);
        mesMaps[i].setSpawnPoint(Vector2f(spawnpoints[i][0], spawnpoints[i][1]));
        mesMaps[i].ajouterElement(&joueur1, JOUEUR);
        mesMaps[i].ajouterElement(&joueur2, JOUEUR);
    }
    current_map = 0;
    joueur1.getSprite().setPosition(mesMaps[TUTO].getSpawnPoint());
    joueur2.getSprite().setPosition(mesMaps[TUTO].getSpawnPoint().x, mesMaps[TUTO].getSpawnPoint().y + 32);

}

void Game::changerMap(int numcarte, Sprite& newspawpoint) {
    if (numcarte > 0) {
        for (int i = 0; i < 200; i++) {
            RectangleShape rectangle(Vector2f(800.f, 600.f));
            rectangle.setPosition(mWindow.getView().getCenter() - mWindow.getView().getSize() / 2.f);
            rectangle.setFillColor(Color(0, 0, 0, 10));
            Time t1 = milliseconds(5);
            mWindow.draw(rectangle);
            sleep(t1);
            mWindow.display();
        }
    }

    getCurrentMap().setSpawnPoint(newspawpoint.getPosition());
    current_map = numcarte;
    joueur1.getSprite().setPosition((Vector2f) getCurrentMap().getSpawnPoint());
    joueur2.getSprite().setPosition(getCurrentMap().getSpawnPoint().x + P_SIZE, getCurrentMap().getSpawnPoint().y);
    vue_jeu.setCenter(joueur1.getPosition());
    render();
}

Map& Game::getCurrentMap() {
    return mesMaps[current_map];
}

vector<vector<int> > Game::getSpawnPoints() {
    ifstream myFile;
    vector<vector<int>> myArray;
    myFile.open("Maps/spawn.txt");
    string line;
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            myArray.push_back(traiteString(line));
        }
        myFile.close();
    } else {
        cout << "Fichier de spawn non ouverts" << endl;
        exit(EXIT_FAILURE);
    }
    return myArray;
}

vector<int> Game::traiteString(string atraiter) {
    vector<int> maliste;
    for (int i = 0; i < atraiter.length(); i++) {
        string total = "";
        while (atraiter[i] != ',' && i < atraiter.length() && atraiter[i] != '\n' && !isspace(atraiter[i])) {
            total += atraiter[i];
            i++;
        }
        int monEntier = atoi(total.c_str());
        maliste.push_back(monEntier);
    }
    return maliste;
}

bool Game::outOfview(Sprite& positJ) {
    Vector2f center_v = vue_jeu.getCenter();
    Vector2f size_v = vue_jeu.getSize();

    float left = center_v.x - size_v.x / 2;
    float right = center_v.x + size_v.x / 2;
    float up = center_v.y - size_v.y / 2;
    float down = center_v.y + size_v.y / 2;

    //cout << " Gauche : " << left << " - Droite : " << right << " - Haut : " << up << "- Bas  : " << down << "== Joueur : x-" << positJ.getPosition().x << "- y" << positJ.getPosition().y << endl;

    return positJ.getPosition().x - P_SIZE < left || positJ.getPosition().x + P_SIZE > right || positJ.getPosition().y + P_SIZE > down || positJ.getPosition().y - P_SIZE / 2 < up;
}

void Game::loading() {
    Texture img;
    if (!img.loadFromFile("menu.png")) {
        cout << "Erreur lors du chargement de l'image de chargement" << endl;
    }
    Sprite backg(img);
    mWindow.draw(backg);
    Font titre_font;
    if (!titre_font.loadFromFile("Pixel-Noir.ttf")) {
        cout << "Erreur lors du chargement de la polices" << endl;
    }
    Text cequisepasse;
    cequisepasse.setPosition(400.f, 300.f);
    cequisepasse.setFont(titre_font);
    cequisepasse.setColor(Color::White);

    changerLoading("Chargement des textures", cequisepasse, backg, mWindow);
    loadTileSet();
    chargementTexture();
    changerLoading("Chargement des cartes ...", cequisepasse, backg, mWindow);
    chargerCartes();
    changerLoading("Fini", cequisepasse,backg, mWindow);
    changerMap(TUTO, joueur1.getSprite());
}

void Game::changerLoading(string texte, Text& achanger, Sprite& fond, RenderWindow& fenetre) {
    fenetre.clear();
    achanger.setPosition(200.f, 150.f);
    achanger.setString(String(texte));
    fond.setPosition(0.f, 0.f);
    fenetre.draw(fond);
    fenetre.draw(achanger);
    fenetre.display();
}

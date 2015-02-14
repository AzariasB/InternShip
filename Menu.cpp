/* 
 * File:   Menu.cpp
 * Author: dorian
 * 
 * Created on 3 février 2015, 10:53
 */

#include "Menu.h"

using namespace sf;

Menu::Menu(sf::RenderWindow &pWindow, 
         const sf::String & backgroundImagepath,
         const sf::String & cursorImagepath,
         const sf::Vector2f & cursorOrigin,
         const sf::String & fontPath,
         const sf::String & instructionsPath,
         const sf::String & creditsPath
         )
: window(pWindow), mHoow(255), mState(MAIN) {
    mBackgroundTex.loadFromFile(backgroundImagepath);
    mBackground.setTexture(mBackgroundTex);
    mCursorTex.loadFromFile(cursorImagepath);
    mCursorTex.setSmooth(true);
    mCursor.setTexture(mCursorTex);
    mCursor.setOrigin(cursorOrigin);
    // move mouse position to window center
    sf::Vector2u winSize = window.getSize();
    sf::Mouse::setPosition(sf::Vector2i(winSize.x, winSize.y)/2, window);

    if (!mFont.loadFromFile(fontPath)) {
        cerr << "Impossible de charger la police '" << fontPath.toAnsiString() <<"'" << endl;
        exit(EXIT_FAILURE);
    }

    mInstructionsTex.loadFromFile(instructionsPath);
    mInstructions.setTexture(mInstructionsTex);
    mCreditsTex.loadFromFile(creditsPath);
    mCredits.setTexture(mCreditsTex);
    
    createTexts();
}

Menu::Choice Menu::run() {
    window.setMouseCursorVisible(false);
    mRunning = true;
    while (mRunning) {
        processEvent();
        render();
    }
    return mChoice;
}

void Menu::render() {
    window.clear();
    switch (mState) {
        case MAIN:
            window.draw(mBackground);
            break;
        case INSTRUCTIONS:
            window.draw(mInstructions);
            break;
        case CREDITS:
            window.draw(mCredits);
            break;
    }
    renderTitle();
    hoow();
    window.draw(mCursor);
    window.display();
}

void Menu::update() {
}

void Menu::processEvent() {
    Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case Event::Closed:
                mRunning = false;
                mChoice = QUIT;
                break;
            case Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    mRunning = false;
                    mChoice = QUIT;
                }
            case Event::MouseMoved:
                handleMouseMove(event);
                break;
            case Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                    handleMouseClick(event);
                break;
        }
    }
}

void Menu::handleMouseMove(Event ev) {
    float x_mouse = ev.mouseMove.x;
    float y_mouse = ev.mouseMove.y;

    mCursor.setPosition(x_mouse, y_mouse);

    // cout << " X : " << x_mouse << " - y :" << y_mouse << endl;
    for (int i = 0; i < text_affiche.size(); i++) {
        float text_witdth = text_affiche[i].getGlobalBounds().width;
        float text_height = text_affiche[i].getGlobalBounds().height;
        float text_x = text_affiche[i].getPosition().x;
        float text_y = text_affiche[i].getPosition().y;
        //// cout << " Text x :" << text_x << " Text y :  " << text_y <<  " - Text width : " << text_witdth << " - Text height : " << text_height << endl;
        if (x_mouse > text_x 
                && x_mouse < (text_x + text_witdth) 
                && y_mouse > text_y 
                && y_mouse < (text_y + text_height)) {
            //// cout << "Coucou " << endl;
            text_affiche[i].setColor(Color::Yellow);
        } else {
            text_affiche[i].setColor(Color::White);
        }
    }
    // cout << "===================" << endl;
}

void Menu::handleMouseClick(Event ev) {
    float x_mouse = ev.mouseButton.x;
    float y_mouse = ev.mouseButton.y;
    // cout << " X : " << x_mouse << " - y :" << y_mouse << endl;
    for (int i = 0; i < text_affiche.size(); i++) {
        float text_witdth = text_affiche[i].getGlobalBounds().width;
        float text_height = text_affiche[i].getGlobalBounds().height;
        float text_x = text_affiche[i].getPosition().x;
        float text_y = text_affiche[i].getPosition().y;

        if (x_mouse > text_x 
                && x_mouse < (text_x + text_witdth) 
                && y_mouse > text_y 
                && y_mouse < (text_y + text_height)) {

            switch (i) {
                case 0:
                    // cout << "Le jeu commence" << endl;
                    mChoice = START_GAME;
                    mRunning = false;
                    break;
                case 1:
                    // cout << "Options" << endl;
                    mState = INSTRUCTIONS;
                    break;
                case 2:
                    // cout << "Crédits" << endl;
                    mState = CREDITS;
                    break;
                case 3:
                    // cout << "Quitter";
                    mChoice = QUIT;
                    mRunning = false;
                    break;
                 case 4:
                    // back btn
                    mState = MAIN;
                    break;
            }

            text_affiche[i].setColor(Color::Yellow);
        } else {
            text_affiche[i].setColor(Color::White);
        }
    }
    // cout << "===================" << endl;
}

void Menu::renderTitle() {
    // cout << text_affiche.size() << endl;
    int max = text_affiche.size();
    if (mState == MAIN) {
        max--;
        for (int i = 0; i < max; i++) {
            Text montext = text_affiche[i];
            // cout << montext.getString().toAnsiString() << endl;
            window.draw(montext);
        }
    } else {
        window.draw(text_affiche[max-1]);
    }
}

void Menu::createTexts() {
    int x_center = 150;
    int y_center = 250;

    vector<string> text{"Demarrer une partie", "Instructions", "Credits", "Quitter le jeu", "Retour"};
    for (int i = 0; i < text.size(); i++) {
        Text menuText;
        menuText.setString(text[i]);
        menuText.setCharacterSize(32);
        menuText.setPosition((50.f) + x_center, i * (50.f) + y_center);
        menuText.setColor(Color::White);
        menuText.setFont(mFont);
        text_affiche.push_back(menuText);
    }
    // set back btn
    text_affiche[text_affiche.size()-1].setPosition(20,550);
}

void Menu::hoow() {
    if (mHoow <= 0)
        return;
    sf::RectangleShape rect(window.getView().getSize());
    rect.setFillColor(sf::Color(0,0,0,mHoow));
    window.draw(rect);
    mHoow -= 10;

}


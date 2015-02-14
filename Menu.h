/* 
 * File:   Menu.h
 * Author: dorian & globuloz
 *
 * Created on 3 février 2015, 10:53
 */

#ifndef MENU_H
#define	MENU_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Menu {
public:
    enum Choice : int {
        START_GAME,
        QUIT
    };

    Menu(sf::RenderWindow & window, 
            const sf::String & backgroundImagepath,
            const sf::String & cursorImagepath,
            const sf::Vector2f & cursorOrigin,
            const sf::String & fontPath,
            const sf::String & instructionsPath,
            const sf::String & creditsPath
            );
    virtual ~Menu(){};
    /**
      Launch the menu.
      Return the user choice.
    */
    Choice run();
private:
    void update();
    void renderTitle();
    void processEvent();
    void render();
    void handleMouseMove(Event e);
    void handleMouseClick(Event e);
    void createTexts();
    void hoow();
    
    RenderWindow & window;
    vector<Text> text_affiche;
    Choice mChoice;
    bool mRunning;
    sf::Texture mBackgroundTex;
    sf::Sprite mBackground;
    sf::Texture mCursorTex;
    sf::Sprite mCursor;
    int mHoow;
    sf::Font mFont;
    sf::Texture mInstructionsTex;
    sf::Sprite mInstructions;
    sf::Texture mCreditsTex;
    sf::Sprite mCredits;

    enum State {MAIN, INSTRUCTIONS, CREDITS};
    State mState;
};

#endif	/* MENU_H */


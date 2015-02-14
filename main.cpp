#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Logo.hpp"
#include "Menu.h"
#include "Game.hpp"

using namespace std;

void displayLogo(sf::RenderWindow & window, const sf::String & imagepath);

int main(int argc, char ** argv) {

    cout << "SFML2 Project" << __DATE__ << " à " << __TIME__ << endl;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Title", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);

    /*
       Animated logos
     */
    displayLogo(window, "sfml-logo-big.png");
    displayLogo(window, "logo-internship.png");


    /*
       Menu
     */
    Menu menu(window,
            "menu.png",
            "cursor.png",
            sf::Vector2f(20, 20),
            "Pixel-Noir.ttf",
            "instructions.png",
            "credits.png"
            );


    bool runningMenu = true;
    while (runningMenu) {
        // choice is like an integer.
        Menu::Choice choice = menu.run();

        switch (choice) {
            case Menu::START_GAME:
            {
                runningMenu = false;
                Game g;
                window.close();
                g.run();

            }

                break;
            case Menu::QUIT:
                runningMenu = false;
                break;
        }
    }

    return 0;
}

void displayLogo(sf::RenderWindow & window, const sf::String & imagepath) {
    sf::Texture texture;
    texture.loadFromFile(imagepath);

    Logo::LogoTexture ltex(texture);
    Logo::LogoAnimation anim(sf::milliseconds(3000));
    Logo::LogoAnimated logoAnimated(ltex, anim, window.getView());

    while (logoAnimated.isPlaying()) {
        logoAnimated.update();
        window.clear(sf::Color::Black);
        window.draw(logoAnimated);
        window.display();
    }
}



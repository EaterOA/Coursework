#include <SFML/Graphics.hpp>
#include <iostream>
#include "StarField.hpp"

sf::RenderWindow window;
sf::Clock appClock;
StarField field;
bool focused;

bool appInit()
{
    srand(unsigned(time(0)));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(800, 600), "Star Simulation", sf::Style::Close, settings);
    window.setFramerateLimit(60);

    field.initCircle(5, 2000, 5);

    return true;
}

void processEvents()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            break;
        }
        else if (event.type == sf::Event::LostFocus) {
            focused = false;
        }
        else if (event.type == sf::Event::GainedFocus) {
            focused = true;
        }
    }
}

void appStart()
{
    //App loop
    while (window.isOpen()) {

        sf::Time elapsed = appClock.restart();
        processEvents();

        if (focused) {
            field.tick(elapsed.asSeconds());
        }

        window.display();
    }
}

int main()
{
    if (!appInit()) {
        std::cerr << "Cannot initialize application. Aborting.\n";
        if (window.isOpen()) window.close();
    }
    else {
        appStart();
    }
}

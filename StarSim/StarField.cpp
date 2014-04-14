#include "StarField.hpp"

int rand(int min, int max)
{
    int range = max - min + 1;
    return std::rand() % range + min;
}

void Star::update(std::vector<Star> &universe)
{
}

void Star::move(float t)
{
}

void StarField::initCircle(int mass, int stars, int galaxies)
{
    int s = stars / galaxies;
    for (int i = 0; i < galaxies; i++) {
        int x = rand(0, 800), y = rand(0, 600);
        for (int j = 0; j < s; j++) {
            float r = rand(100, 1500)/10.f;
            float rad = rand(1, 3600)/10.f;

        }

    }
}

void StarField::tick(float t)
{
    for (unsigned i = 0; i < universe.size(); i++)
        universe[i].update(universe);
    for (unsigned i = 0; i < universe.size(); i++)
        universe[i].move(t);
}

void StarField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

#ifndef STARFIELD_HPP
#define STARFIELD_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Star
{
public:
    void update(std::vector<Star>& universe);
    void move(float t);
private:
    sf::Vector2f m_pos, m_vel;
    int m_mass;
};

class StarField: public sf::Drawable, public sf::Transformable
{
public:
    void initCircle(int mass, int stars, int galaxies);
    void tick(float t);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<Star> universe;
};

#endif
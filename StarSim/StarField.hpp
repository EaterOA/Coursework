#ifndef STARFIELD_HPP
#define STARFIELD_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Star
{
public:
    Star(float mass, sf::Vector2f pos);
    void update(std::vector<Star>& universe);
    void move(float t);
    sf::Vector2f getPos() const;
private:
    sf::Vector2f m_pos, m_vel, m_acc;
    float m_mass;
};

class StarField: public sf::Drawable, public sf::Transformable
{
public:
    void initCircle(float mass, int stars, int galaxies);
    void tick(float t);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<Star> m_universe;
    std::vector<sf::Vertex> m_vertices;
};

#endif

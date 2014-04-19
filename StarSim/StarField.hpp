#ifndef STARFIELD_HPP
#define STARFIELD_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class StarField;

class Star
{
public:
    Star(float mass, sf::Vector2f pos, sf::Vector2f vel);
    void update(const StarField &f, std::vector<Star>& target);
    void move(float t);
    sf::Vector2f getPos() const;
    float getMass() const;
private:
    sf::Vector2f getGravAccTo(Star &t);

    sf::Vector2f m_pos, m_vel, m_acc;
    float m_mass;
};

class StarField: public sf::Drawable, public sf::Transformable
{
public:
    enum FieldMode { STARCIRCLE, GALAXYCENTER };

    void initStarCircle(float mass, float spd, float max_a, int stars, int galaxies);
    void initGalaxyCenter(float mass_g, float mass_s, int stars, int galaxies);
    sf::Vector2f getGravAcc(Star &s, Star &t) const;
    void tick(float t);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    FieldMode m_mode;
    float m_maxAcc;
    std::vector<Star> m_stars;
    std::vector<Star> m_galaxies;
    std::vector<sf::Vertex> m_vertices;
};

#endif

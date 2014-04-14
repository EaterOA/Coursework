#include "Util.hpp"
#include "StarField.hpp"

sf::Vector2f getGravAccTo(Star &s, Star &t)
{
    sf::Vector2f acc;
//    float a = s.getMass() * t.getMass()
    return acc;
}

Star::Star(float mass, sf::Vector2f pos)
{
    m_mass = mass;
    m_pos = pos;
}

void Star::update(std::vector<Star> &universe)
{
    m_acc = sf::Vector2f();
    for (unsigned i = 0; i < universe.size(); i++) {

    }
}

void Star::move(float t)
{
    m_vel += m_acc;
    m_pos += m_vel;
}

sf::Vector2f Star::getPos() const
{
    return m_pos;
}

void StarField::initCircle(float mass, int stars, int galaxies)
{
    int s = stars / galaxies;
    for (int i = 0; i < galaxies; i++) {
        int x = RAND(0, 800), y = RAND(0, 600);
        for (int j = 0; j < s; j++) {
            float r = RAND(100, 1500)/10.f;
            float rad = RAND(1, 3600)/10.f;

        }

    }
}

void StarField::tick(float t)
{
    m_vertices = std::vector<sf::Vertex>(m_universe.size());
    for (unsigned i = 0; i < m_universe.size(); i++)
        m_universe[i].update(m_universe);
    for (unsigned i = 0; i < m_universe.size(); i++) {
        m_universe[i].move(t);
        m_vertices[i].position = m_universe[i].getPos();
    }
}

void StarField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_vertices.empty())
        target.draw(&m_vertices[0], m_vertices.size(), sf::Points);
}

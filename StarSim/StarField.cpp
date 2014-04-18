#include "Util.hpp"
#include "StarField.hpp"

Star::Star(float mass, sf::Vector2f pos, sf::Vector2f vel)
{
    m_mass = mass;
    m_pos = pos;
    m_vel = vel;
    m_acc = sf::Vector2f();
}

void Star::update(const StarField &f, std::vector<Star> &universe)
{
    m_acc = sf::Vector2f();
    for (unsigned i = 0; i < universe.size(); i++) {
        if (this == &universe[i]) continue;
        m_acc += f.getGravAcc(*this, universe[i]);
    }
}

void Star::move(float t)
{
    m_vel += m_acc * t;
    m_pos += m_vel * t;
}

sf::Vector2f Star::getPos() const
{
    return m_pos;
}

float Star::getMass() const
{
    return m_mass;
}

void StarField::initCircle(float mass, float spd, float max_a, int stars, int galaxies)
{
    m_maxAcc = max_a;
    int s = stars / galaxies;
    for (int i = 0; i < galaxies; i++) {
        sf::Vector2f base(RAND(0, 800), RAND(0, 600));
        for (int j = 0; j < s; j++) {
            float r = RAND(100, 1500)/10.f;
            if (RAND(0,1)) spd = -spd;
            float rad = util::toRad(RAND(1, 3600)/10.f);
            sf::Vector2f pos = base + sf::Vector2f(r*cos(rad), r*sin(rad));
            sf::Vector2f upos = util::getUnitVector(base, pos);
            sf::Vector2f uvel = util::rotate(upos, util::toRad(90.f));
            sf::Vector2f vel(spd*uvel.x, spd*uvel.y);
            m_universe.push_back(Star(mass, pos, vel));
        }

    }
}

sf::Vector2f StarField::getGravAcc(Star &s, Star &t) const
{
    sf::Vector2f sp = s.getPos(), tp = t.getPos();
    float d = util::getDist(sp, tp);
    float a = s.getMass() * t.getMass() / pow(util::getDist(sp, tp), 2.0);
    a = MIN(m_maxAcc, a);
    float rad = util::toDir(tp.x - sp.x, tp.y - sp.y);
    return a * sf::Vector2f(cos(rad), sin(rad));
}


void StarField::tick(float t)
{
    m_vertices = std::vector<sf::Vertex>(m_universe.size());
    for (unsigned i = 0; i < m_universe.size(); i++)
        m_universe[i].update(*this, m_universe);
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

#include "Util.hpp"
#include "StarField.hpp"

Star::Star(float mass, sf::Vector2f pos, sf::Vector2f vel)
{
    m_mass = mass;
    m_pos = pos;
    m_vel = vel;
    m_acc = sf::Vector2f();
}

void Star::update(const StarField &f, std::vector<Star> &target)
{
    m_acc = sf::Vector2f();
    for (unsigned i = 0; i < target.size(); i++) {
        if (this == &target[i]) continue;
        m_acc += f.getGravAcc(*this, target[i]);
    }
}

void Star::move(float t)
{
    m_vel += m_acc * t;
    while (util::getLen(m_vel) > 500.f) m_vel = m_vel * 0.9f;
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

void StarField::initStarCircle(float mass, float spd, float max_a, int stars, int galaxies)
{
    m_mode = STARCIRCLE;
    m_maxAcc = max_a;
    int s = stars / galaxies;
    for (int i = 0; i < galaxies; i++) {
        sf::Vector2f base(RAND(0, 800), RAND(0, 600));
        for (int j = 0; j < s; j++) {
            float r = RAND(300, 1000)/10.f;
            if (RAND(0,1)) spd = -spd;
            float rad = util::toRad(RAND(1, 3600)/10.f);
            sf::Vector2f pos = base + sf::Vector2f(r*cos(rad), r*sin(rad));
            sf::Vector2f upos = util::getUnitVector(base, pos);
            sf::Vector2f uvel = util::rotate(upos, util::toRad(90.f));
            sf::Vector2f vel(spd*uvel.x, spd*uvel.y);
            m_stars.push_back(Star(mass, pos, vel));
        }

    }
}

void StarField::initGalaxyCenter(float mass_g, float mass_s, int stars, int galaxies) 
{
    m_mode = GALAXYCENTER;
    int s = stars / galaxies;
    for (int i = 0; i < galaxies; i++) {
        sf::Vector2f base(RAND(0, 800), RAND(0, 600));
        sf::Vector2f drift(RAND(-30,30), RAND(-30,30));
        for (int j = 0; j < s; j++) {
            float r = RAND(300, 900)/10.f;
            float spd = sqrt((mass_g + mass_s)/r);
            float rad = util::toRad(RAND(1, 3600)/10.f);
            sf::Vector2f pos = base + sf::Vector2f(r*cos(rad), r*sin(rad));
            sf::Vector2f upos = util::getUnitVector(base, pos);
            sf::Vector2f uvel = util::rotate(upos, util::toRad(90.f));
            sf::Vector2f vel = drift + sf::Vector2f(spd*uvel.x, spd*uvel.y);
            m_stars.push_back(Star(mass_s, pos, vel));
        }
        m_galaxies.push_back(Star(mass_g, base, drift));
    }
}


sf::Vector2f StarField::getGravAcc(Star &s, Star &t) const
{
    sf::Vector2f sp = s.getPos(), tp = t.getPos();
    float d = util::getDist(sp, tp);
    float a = s.getMass() * t.getMass() / pow(d, 2.0);
    float rad = util::toDir(tp.x - sp.x, tp.y - sp.y);
    return a * sf::Vector2f(cos(rad), sin(rad));
}


void StarField::tick(float t)
{
    m_vertices = std::vector<sf::Vertex>(m_stars.size());
    if (m_mode == STARCIRCLE) {
        for (unsigned i = 0; i < m_stars.size(); i++)
            m_stars[i].update(*this, m_stars);
    }
    else if (m_mode == GALAXYCENTER) {
        for (unsigned i = 0; i < m_stars.size(); i++)
            m_stars[i].update(*this, m_galaxies);
    }
    for (unsigned i = 0; i < m_stars.size(); i++) {
        m_stars[i].move(t);
        m_vertices[i].position = m_stars[i].getPos();
    }
    for (unsigned i = 0; i < m_galaxies.size(); i++)
        m_galaxies[i].move(t);
}

void StarField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_vertices.empty())
        target.draw(&m_vertices[0], m_vertices.size(), sf::Points);
}

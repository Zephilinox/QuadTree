#include "Entity.hpp"

Entity::Entity(sf::Vector2f pos, float radius)
    : m_shape(radius)
{
    m_shape.setOrigin(sf::Vector2f(radius, radius));
    m_shape.setPosition(pos);
    m_shape.setOutlineThickness(1);
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setFillColor(sf::Color::Red);

    m_velocity = zge::Vector::degToVector(std::rand() % 360);
}

Entity::~Entity()
{
    std::cout << "Ent Died [" << this << "]\n";
}

void Entity::update(float dt)
{
    m_shape.move(m_velocity.x * 10 * dt, m_velocity.y * 10 * dt);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
}

void Entity::setPosition(sf::Vector2f pos)
{
    m_shape.setPosition(pos);
}

void Entity::setRadius(float rad)
{
    m_shape.setRadius(rad);
}

sf::Vector2f Entity::getPosition()
{
    return m_shape.getPosition();
}

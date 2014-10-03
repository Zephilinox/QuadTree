#include "Entity.hpp"

Entity::Entity(sf::Vector2f pos, float radius)
    : m_position(pos)
    , m_radius(radius)
    , m_shape(m_radius)
{
    m_shape.setOrigin(sf::Vector2f(radius, radius));
    m_shape.setPosition(m_position);
    m_shape.setOutlineThickness(1);
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setFillColor(sf::Color::Red);
}

void Entity::update(float dt)
{

}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
}

void Entity::setPosition(sf::Vector2f pos)
{

}

void Entity::setRadius(float rad)
{

}

sf::Vector2f Entity::getPosition()
{
    return m_position;
}

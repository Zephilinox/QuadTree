#include "Entity.hpp"

Entity::Entity(float x, float y, float radius)
    : m_position(x, y)
    , m_radius(radius)
    , m_shape(m_radius)
{
    m_shape.setPosition(m_position);
    m_shape.setOutlineThickness(1);
    m_shape.setOutlineColor(sf::Color::Black);
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

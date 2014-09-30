#include "Entity.hpp"

Entity::Entity(float x, float y, float radius)
    : m_position(x, y)
    , m_radius(radius)
{
    //ctor
}

void Entity::update(float dt)
{

}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}

void Entity::setPosition(sf::Vector2f pos)
{

}

void Entity::setRadius(float rad)
{

}

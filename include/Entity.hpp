#ifndef ENTITY_HPP
#define ENTITY_HPP

//STD

//3RD
#include <SFML/Graphics.hpp>

//SELF

class Entity : public sf::Drawable
{
public:
    Entity(float x, float y, float radius);
    void update(float dt);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(sf::Vector2f pos);
    void setRadius(float rad);

    sf::Vector2f getPosition();

private:
    sf::Vector2f m_position;
    float m_radius;
    sf::CircleShape m_shape;
};

#endif // ENTITY_HPP

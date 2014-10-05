#ifndef ENTITY_HPP
#define ENTITY_HPP

//STD

//3RD
#include <SFML/Graphics.hpp>
#include <ZGE/Vector.hpp>

//SELF

class Entity : public sf::Drawable
{
public:
    Entity(sf::Vector2f pos, float radius);
    ~Entity();

    void update(float dt);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(sf::Vector2f pos);
    void setRadius(float rad);

    sf::Vector2f getPosition();

private:
    zge::Vector m_velocity;
    sf::CircleShape m_shape;
};

#endif // ENTITY_HPP

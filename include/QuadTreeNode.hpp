#ifndef QUADTREENODE_HPP
#define QUADTREENODE_HPP

//STD
#include <vector>

//3RD
#include <SFML/Graphics.hpp>

//SELF
#include "Entity.hpp"

class QuadTreeNode
{
public:
    QuadTreeNode();
    ~QuadTreeNode();

    void drawBoundaries(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawEntities(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(float dt);

    bool addEntity(Entity ent);

    bool isUseless();

private:
    void subdivide();

    bool m_isLeaf;

    QuadTreeNode* m_rootNode;

    QuadTreeNode* m_topLeft;
    QuadTreeNode* m_topRight;
    QuadTreeNode* m_bottomRight;
    QuadTreeNode* m_bottomLeft;

    sf::RectangleShape m_shape;
    sf::FloatRect m_boundary;
    const unsigned m_maxEntities;
    std::vector<Entity> m_entities;
};

#endif //QUADTREENODE_HPP

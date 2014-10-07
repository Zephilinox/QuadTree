#ifndef QUADTREENODE_HPP
#define QUADTREENODE_HPP

//STD
#include <vector>
#include <memory>

//3RD
#include <SFML/Graphics.hpp>

//SELF
#include "Entity.hpp"

class QuadTreeNode
{
public:
    QuadTreeNode(QuadTreeNode* rootNode, sf::FloatRect boundary, unsigned maxEnts);
    ~QuadTreeNode();

    void drawBoundaries(sf::RenderTarget& target) const;
    void drawEntities(sf::RenderTarget& target) const;
    void update(float dt);

    bool addEntity(std::shared_ptr<Entity> ent);

    bool isUseless();

    void killChildren();
private:
    void drawChildBoundaries(sf::RenderTarget& target) const;
    void drawChildEntities(sf::RenderTarget& target) const;

    void spawnChildren();
    bool areChildrenUseless();
    void giveEntitiesToChildren();

    const unsigned m_maxEntities;
    bool m_isLeaf;

    QuadTreeNode* m_rootNode;

    QuadTreeNode* m_topLeft;
    QuadTreeNode* m_topRight;
    QuadTreeNode* m_bottomRight;
    QuadTreeNode* m_bottomLeft;

    sf::RectangleShape m_shape;
    sf::FloatRect m_boundary;
    std::vector<std::shared_ptr<Entity>> m_entities;
};

#endif //QUADTREENODE_HPP

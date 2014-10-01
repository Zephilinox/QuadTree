#include "QuadTreeNode.hpp"

QuadTreeNode::QuadTreeNode()
    : m_maxEntities(4)
{
}

QuadTreeNode::~QuadTreeNode()
{

}

void QuadTreeNode::drawBoundaries(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
}

void QuadTreeNode::drawEntities(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const Entity& ent : m_entities)
    {
        target.draw(ent, states);
    }
}

void QuadTreeNode::update(float dt)
{

}

bool QuadTreeNode::addEntity(Entity ent)
{
    return false;
}

bool QuadTreeNode::isUseless()
{
    return false;
}

#include "QuadTreeNode.hpp"

QuadTreeNode::QuadTreeNode()
    : m_maxEntities(4)
    , m_isLeaf(true)
{
}

QuadTreeNode::~QuadTreeNode()
{
    killChildren();
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
    if (m_boundary.contains(ent.getPosition()))
    {
        if (m_isLeaf)
        {
            if (m_maxEntities <= m_entities.size())
            {
                m_entities.push_back(ent);
                return true;
            }
            else
            {
                spawnChildren();
                addEntity(ent);

                return true;
            }
        }
        else
        {
            m_topLeft->addEntity(ent);
            m_topRight->addEntity(ent);
            m_bottomLeft->addEntity(ent);
            m_bottomRight->addEntity(ent);

            return true;
        }
    }

    return false;
}

bool QuadTreeNode::isUseless()
{
    return (m_isLeaf && m_entities.size() == 0);
}

void QuadTreeNode::spawnChildren()
{
    if (m_isLeaf)
    {
        m_isLeaf = false;

        m_topLeft = new QuadTreeNode();
        m_topRight = new QuadTreeNode();
        m_bottomRight = new QuadTreeNode();
        m_bottomLeft = new QuadTreeNode();
    }
}

void QuadTreeNode::killChildren()
{
    m_isLeaf = true;

    delete m_topLeft;
    delete m_topRight;
    delete m_bottomRight;
    delete m_bottomLeft;

    m_topLeft = nullptr;
    m_topRight = nullptr;
    m_bottomRight = nullptr;
    m_bottomLeft = nullptr;
}

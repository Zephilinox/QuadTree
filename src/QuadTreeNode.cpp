#include "QuadTreeNode.hpp"

//STD
#include <iostream>

QuadTreeNode::QuadTreeNode(QuadTreeNode* rootNode, sf::FloatRect boundary, unsigned maxEnts)
    : m_maxEntities(maxEnts)
    , m_isLeaf(true)
    , m_rootNode((rootNode ? rootNode : this))
    , m_topLeft(nullptr)
    , m_topRight(nullptr)
    , m_bottomLeft(nullptr)
    , m_bottomRight(nullptr)
    , m_boundary(boundary)
{
    m_shape.setPosition(boundary.left, boundary.top);
    m_shape.setSize(sf::Vector2f(boundary.width, boundary.height));
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setOutlineThickness(-1);
}

QuadTreeNode::~QuadTreeNode()
{
    killChildren();
}

void QuadTreeNode::drawBoundaries(sf::RenderTarget& target) const
{
    target.draw(m_shape);

    drawChildBoundaries(target);
}

void QuadTreeNode::drawEntities(sf::RenderTarget& target) const
{
    for (const auto& ent : m_entities)
    {
        target.draw(*ent);
    }

    drawChildEntities(target);
}

void QuadTreeNode::update(float dt)
{
    if (areChildrenUseless())
    {
        killChildren();
    }

    if (m_isLeaf)
    {
        //Erase entity if lambda returns true, which it will do after updating each entity and checking if they are within the boundary
        m_entities.erase(
            std::remove_if(
                m_entities.begin(),
                m_entities.end(),
                [this, dt](std::shared_ptr<Entity> e)
                {
                    e->update(dt);

                    if (!m_boundary.contains(e->getPosition()))
                    {
                        m_rootNode->addEntity(e);

                        return true;
                    }

                    return false;
                }
            ),
            m_entities.end()
        );
    }
    else //Not leaf, thus contains nodes
    {
        m_topLeft->update(dt);
        m_topRight->update(dt);
        m_bottomLeft->update(dt);
        m_bottomRight->update(dt);
    }
}

bool QuadTreeNode::addEntity(std::shared_ptr<Entity> ent)
{
    if (m_boundary.contains(ent->getPosition()))
    {
        if (m_isLeaf)
        {
            if (m_maxEntities > m_entities.size())
            {
                m_entities.push_back(ent);
                return true;
            }
            else
            {
                spawnChildren();
            }
        }

        if (m_topLeft->addEntity(ent) ||
            m_topRight->addEntity(ent) ||
            m_bottomLeft->addEntity(ent) ||
            m_bottomRight->addEntity(ent))
        {
            return true;
        }
    }

    if (m_rootNode == this) //As the shared_ptr is not stored anywhere, once this returns the entity will be automatically cleaned up.
    {
        std::cout << "Ent is not within quadtree boundaries\n";
    }

    return false;
}

bool QuadTreeNode::isUseless()
{
    return (m_isLeaf && m_entities.size() == 0);
}

void QuadTreeNode::drawChildBoundaries(sf::RenderTarget& target) const
{
    if (m_isLeaf) return;

    m_topLeft->drawBoundaries(target);
    m_topRight->drawBoundaries(target);
    m_bottomRight->drawBoundaries(target);
    m_bottomLeft->drawBoundaries(target);
}

void QuadTreeNode::drawChildEntities(sf::RenderTarget& target) const
{
    if (m_isLeaf) return;

    m_topLeft->drawEntities(target);
    m_topRight->drawEntities(target);
    m_bottomRight->drawEntities(target);
    m_bottomLeft->drawEntities(target);
}

void QuadTreeNode::spawnChildren()
{
    if (m_isLeaf)
    {
        m_isLeaf = false;

        m_topLeft = new QuadTreeNode(m_rootNode, sf::FloatRect(m_boundary.left, m_boundary.top, m_boundary.width/2, m_boundary.height/2), m_maxEntities);
        m_topRight = new QuadTreeNode(m_rootNode, sf::FloatRect(m_boundary.left + m_boundary.width/2, m_boundary.top, m_boundary.width/2, m_boundary.height/2), m_maxEntities);
        m_bottomRight = new QuadTreeNode(m_rootNode, sf::FloatRect(m_boundary.left + m_boundary.width/2, m_boundary.top + m_boundary.height/2, m_boundary.width/2, m_boundary.height/2), m_maxEntities);
        m_bottomLeft = new QuadTreeNode(m_rootNode, sf::FloatRect(m_boundary.left, m_boundary.top + m_boundary.height/2, m_boundary.width/2, m_boundary.height/2), m_maxEntities);

        giveEntitiesToChildren();
    }
}

void QuadTreeNode::killChildren()
{
    if (!m_isLeaf)
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
}

bool QuadTreeNode::areChildrenUseless()
{
    if (m_isLeaf) return false;

    return m_topLeft->isUseless() && m_topRight->isUseless() && m_bottomLeft->isUseless() && m_bottomRight->isUseless();
}

void QuadTreeNode::giveEntitiesToChildren()
{
    if (m_isLeaf) return;

    for (auto ent : m_entities)
    {
        m_topLeft->addEntity(ent);
        m_topRight->addEntity(ent);
        m_bottomLeft->addEntity(ent);
        m_bottomRight->addEntity(ent);
    }

    m_entities.clear();
}

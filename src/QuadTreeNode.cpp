#include "QuadTreeNode.hpp"

//STD
#include <iostream>

QuadTreeNode::QuadTreeNode(QuadTreeNode* rootNode, sf::FloatRect boundary, unsigned maxEnts)
    : m_maxEntities(maxEnts)
    , m_isLeaf(true)
    , m_boundary(boundary)
{
    if (m_rootNode == nullptr)
    {
        m_rootNode = this;
    }

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

    for (auto& ent : m_entities)
    {
        ent->update(dt);
    }

    /*if (m_isLeaf)
    {
        m_entities.erase(
            std::remove_if(
                m_entities.begin(),
                m_entities.end(),
                [this](std::shared_ptr<Entity> e) //Reference: ent is lost when adding to root; Value: crashes when adding to root
                {
                    if (!m_boundary.contains(e->getPosition()))
                    {
                        if (m_rootNode)
                        {
                            std::cout << "Adding ent to root\n";
                            m_rootNode->addEntity(e); //something wrong here
                        }

                        return true;
                    }

                    return false;
                }
            ),
            m_entities.end()
        );*/
    for (auto& e : m_entities)
    {
        if (!m_boundary.contains(e->getPosition()))
        {
            if (m_rootNode)
            {
                //somehow this works... 20% of the time, still crashes...
                std::cout << "Adding ent to root\n";
                m_rootNode->addEntity(e);

            }
        }
    }

    if (!m_isLeaf)
    {
        m_topLeft->update(dt);
        m_topRight->update(dt);
        m_bottomLeft->update(dt);
        m_bottomRight->update(dt);
    }
}

bool QuadTreeNode::addEntity(std::shared_ptr<Entity> ent)
{
    std::cout << "Adding ent " << ent.get() << "\n";

    //Crashes here; I will sort out how I store entities in the future to stop constructing and destructing them all the time, maybe that is the issue
    if (m_boundary.contains(ent->getPosition()))
    {
        std::cout << "contains ent\n";
        if (m_isLeaf)
        {
            std::cout << "is leaf\n";
            if (m_maxEntities > m_entities.size())
            {
                std::cout << "Ent added " << ent.get() << "\n";
                m_entities.push_back(ent);
                return true;
            }
            else
            {
                std::cout << "Spawning children\n";
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
        bool success = false;
        success |= m_topLeft->addEntity(ent);
        success |= m_topRight->addEntity(ent);
        success |= m_bottomLeft->addEntity(ent);
        success |= m_bottomRight->addEntity(ent);

        std::cout << (success ? "Ent added to children\n" : "Failed to add ent to children\n");
    }

    m_entities.clear();
}

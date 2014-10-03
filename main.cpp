
//STD
#include <iostream>

//3RD
#include <SFML/Graphics.hpp>

//SELF
#include "Entity.hpp"
#include "QuadTreeNode.hpp"

//Bug: so much subdivision that there is no space to store points, so they get deleted. need to set max depth and stop subdividing at that point;

class Point;
class Node;
class QuadTree;

class Point : public sf::Drawable
{
public:
    Point(float posX, float posY, float rad = 8)
        : x(posX)
        , y(posY)
        , radius(rad)
    {
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        sf::CircleShape shape(radius);
        shape.setPosition(x, y);
        shape.setOutlineThickness(1);
        shape.setOutlineColor(sf::Color::Black);
        target.draw(shape, states);
    }

    void update(float dt)
    {
        x += 10 * dt;
        y += 10 * dt;
    }

    float x;
    float y;
    float radius;

private:
    unsigned m_angle;
};

class Node
{
public:
    Node(Node* root, sf::FloatRect boundary, unsigned maxPoints)
        : m_boundary(boundary)
        , m_maxPoints(maxPoints)
        , m_root(root)
        , m_NW(nullptr)
        , m_NE(nullptr)
        , m_SE(nullptr)
        , m_SW(nullptr)
    {
        if (!m_root)
        {
            m_root = this;
        }
    }

    ~Node()
    {
        killChildren();
    }

    void drawBoundaries(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::RectangleShape shape(sf::Vector2f(m_boundary.width, m_boundary.height));
        shape.setPosition(m_boundary.left, m_boundary.top);
        shape.setOutlineThickness(-1);
        shape.setOutlineColor(sf::Color::Black);
        target.draw(shape, states);

        if (m_NW)
        {
            m_NW->drawBoundaries(target, states);
        }

        if (m_NE)
        {
            m_NE->drawBoundaries(target, states);
        }

        if (m_SE)
        {
            m_SE->drawBoundaries(target, states);
        }

        if (m_SW)
        {
            m_SW->drawBoundaries(target, states);
        }
    }

    void drawPoints(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (const Point& p : m_points)
        {
            target.draw(p, states);
        }

        if (m_NW)
        {
            m_NW->drawPoints(target, states);
        }

        if (m_NE)
        {
            m_NE->drawPoints(target, states);
        }

        if (m_SE)
        {
            m_SE->drawPoints(target, states);
        }

        if (m_SW)
        {
            m_SW->drawPoints(target, states);
        }
    }

    void update(float dt)
    {
        if (m_NW && m_NE && m_SE && m_SW)
        {
            if (m_NW->isUseless() && m_NE->isUseless() && m_SE->isUseless() && m_SW->isUseless())
            {
                killChildren();
            }
        }

        m_points.erase(
            std::remove_if(
                m_points.begin(),
                m_points.end(),
                [this, dt](Point& p)
                {
                    p.update(dt);

                    if (!m_boundary.contains(p.x, p.y))
                    {
                        if (m_root)
                        {
                            m_root->addPoint(p);
                        }

                        return true;
                    }

                    return false;
                }
            ),
            m_points.end()
        );

        if (m_NW)
        {
            m_NW->update(dt);
        }

        if (m_NE)
        {
            m_NE->update(dt);
        }

        if (m_SE)
        {
            m_SE->update(dt);
        }

        if (m_SW)
        {
            m_SW->update(dt);
        }
    }

    bool isUseless()
    {
        return (m_maxPoints != 0 && m_points.size() == 0);
    }

    void subdivide()
    {
        m_NW = new Node(m_root, sf::FloatRect(m_boundary.left, m_boundary.top, m_boundary.width/2, m_boundary.height/2), m_maxPoints);
        m_NE = new Node(m_root, sf::FloatRect(m_boundary.left + m_boundary.width/2, m_boundary.top, m_boundary.width/2, m_boundary.height/2), m_maxPoints);
        m_SE = new Node(m_root, sf::FloatRect(m_boundary.left + m_boundary.width/2, m_boundary.top + m_boundary.height/2, m_boundary.width/2, m_boundary.height/2), m_maxPoints);
        m_SW = new Node(m_root, sf::FloatRect(m_boundary.left, m_boundary.top + m_boundary.height/2, m_boundary.width/2, m_boundary.height/2), m_maxPoints);

        for (Point p : m_points)
        {
            m_NW->addPoint(p);
            m_NE->addPoint(p);
            m_SE->addPoint(p);
            m_SW->addPoint(p);
        }

        m_points.clear();
        m_maxPoints = 0;
    }

    void killChildren()
    {
        delete m_NW;
        delete m_NE;
        delete m_SE;
        delete m_SW;
        m_NW = nullptr;
        m_NE = nullptr;
        m_SE = nullptr;
        m_SW = nullptr;
        m_maxPoints = 4;
    }

    bool addPoint(Point p)
    {
        if (m_boundary.contains(p.x, p.y))
        {
            if (m_points.size() < m_maxPoints)
            {
                m_points.push_back(p);
                return true;
            }

            if (!m_NW)
            {
                subdivide();
            }

            if (m_NW && m_NW->addPoint(p))
            {
                return true;
            }

            if (m_NE && m_NE->addPoint(p))
            {
                return true;
            }

            if (m_SE && m_SE->addPoint(p))
            {
                return true;
            }

            if (m_SW && m_SW->addPoint(p))
            {
                return true;
            }
        }

        return false;
    }

private:
    sf::FloatRect m_boundary;
    unsigned m_maxPoints;
    std::vector<Point> m_points;

    Node* m_root;
    Node* m_NW;
    Node* m_NE;
    Node* m_SE;
    Node* m_SW;
};

class QuadTree : public sf::Drawable
{
public:
    QuadTree()
        : m_root(nullptr, sf::FloatRect(0, 0, 800, 800), 4)
    {

    }

    void update(float dt)
    {
        m_root.update(dt);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        m_root.drawBoundaries(target, states);
        m_root.drawPoints(target, states);
    }

    bool addPoint(Point p)
    {
        return m_root.addPoint(p);
    }

private:
    Node m_root;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800, 32), "QuadTree");
    window.setVerticalSyncEnabled(true);

    //QuadTree quadTree;
    QuadTreeNode quadTree(nullptr, sf::FloatRect(0, 0, 800, 800), 4);

    sf::Event event;
    sf::Time prevFrameTime(sf::seconds(1.f / 60.f));
    sf::Clock frameClock;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    /*Point p(event.mouseButton.x, event.mouseButton.y);
                    p.x -= p.radius;
                    p.y -= p.radius;

                    quadTree.addPoint(p);*/
                    Entity e(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), 8);
                    quadTree.addEntity(e);
                    std::cout << "\n\n";
                }

                default:
                {
                    break;
                }
            }
        }

        quadTree.update(prevFrameTime.asSeconds());

        window.clear(sf::Color::Black);
        quadTree.drawBoundaries(window);
        quadTree.drawEntities(window);
        //window.draw(quadTree);
        window.display();

        //std::cout << "FPS: " << 1.f / prevFrameTime.asSeconds() << "\n";
        prevFrameTime = frameClock.restart();
    }

    return 0;
}

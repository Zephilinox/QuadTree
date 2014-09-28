
//STD
#include <iostream>

//3RD
#include <SFML/Graphics.hpp>

class Point : public sf::Drawable
{
public:
    Point(float posX, float posY, float rad = 8)
        : x(posX - rad)
        , y(posY - rad)
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

    float x;
    float y;
    float radius;

private:
};

class Node : public sf::Drawable
{
public:
    Node(sf::FloatRect boundary, unsigned maxPoints)
        : m_boundary(boundary)
        , m_maxPoints(maxPoints)
        , m_NW(nullptr)
        , m_NE(nullptr)
        , m_SE(nullptr)
        , m_SW(nullptr)
    {

    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        sf::RectangleShape shape(sf::Vector2f(m_boundary.width, m_boundary.height));
        shape.setOutlineThickness(-1);
        shape.setOutlineColor(sf::Color::Black);
        target.draw(shape, states);

        for (Point& p : m_points)
        {
            target.draw(p, states);
        }

        if (m_NW)
        {
            target.draw(*m_NW, states);
        }

        if (m_NE)
        {
            target.draw(*m_NE, states);
        }

        if (m_SE)
        {
            target.draw(*m_SE, states);
        }

        if (m_SW)
        {
            target.draw(*m_SW, states);
        }
    }

    bool addPoint(Point p)
    {
        if (m_boundary.contains(p.x, p.y))
        {
            m_points.push_back(p);
            return true;
        }

        return false;
    }

private:
    sf::FloatRect m_boundary;
    const unsigned m_maxPoints;
    mutable std::vector<Point> m_points;

    Node* m_NW;
    Node* m_NE;
    Node* m_SE;
    Node* m_SW;
};

class QuadTree : public sf::Drawable
{
public:
    QuadTree()
        : m_root(sf::FloatRect(0, 0, 800, 800), 4)
    {

    }

    void update(float dt)
    {

    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(m_root, states);
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

    QuadTree quadTree;

    sf::Event event;
    sf::Time prevFrameTime(sf::seconds(1.f / 60.f));
    sf::Clock frameClock;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::MouseButtonPressed:
                {
                    Point p (event.mouseButton.x, event.mouseButton.y);

                    std::cout << quadTree.addPoint(p) << "\n";
                }

                default:
                {
                    break;
                }
            }
        }

        quadTree.update(prevFrameTime.asSeconds());

        window.clear(sf::Color::Black);
        window.draw(quadTree);
        window.display();

        std::cout << "FPS: " << 1.f / prevFrameTime.asSeconds() << "\n";
        prevFrameTime = frameClock.restart();
    }

    return 0;
}

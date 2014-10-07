
//STD
#include <iostream>
#include <memory>

//3RD
#include <SFML/Graphics.hpp>

//SELF
#include "Entity.hpp"
#include "QuadTreeNode.hpp"

//Bug: so much subdivision that there is no space to store points, so they get deleted. need to set max depth and stop subdividing at that point;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800, 32), "QuadTree");
    window.setVerticalSyncEnabled(true);

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
                    std::shared_ptr<Entity> e(new Entity(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), 8));
                    quadTree.addEntity(e);
                }

                case sf::Event::KeyPressed:
                {
                    if (event.key.code == sf::Keyboard::R)
                    {
                        quadTree.killChildren();
                    }
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
        quadTree.drawEntities(window);
        window.display();

        //std::cout << "FPS: " << 1.f / prevFrameTime.asSeconds() << "\n";
        prevFrameTime = frameClock.restart();
        std::cout << "\n\n";
    }

    return 0;
}

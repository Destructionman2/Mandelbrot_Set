#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

int main()
{
    // Get the desktop resolution and divide by 2 for a smaller screen
    const int desktopWidth = sf::VideoMode::getDesktopMode().width / 2;
    const int desktopHeight = sf::VideoMode::getDesktopMode().height / 2;

    // Construct the RenderWindow
    sf::RenderWindow window(sf::VideoMode(desktopWidth, desktopHeight), "Mandelbrot Set");

    // Construct the ComplexPlane
    ComplexPlane complexPlane(desktopWidth, desktopHeight);

    // Construct the Font and Text objects
    sf::Font font;
    font.loadFromFile("KOMIKAP_.ttf"); // You may need to provide the correct path to the font file but its KOMIKAP_.ttf
    sf::Text text("testing", font, 14);
    text.setPosition(10, 10);
    text.setFillColor(sf::Color::White);

    // Main loop
    while (window.isOpen())
    {
        // Handle Input segment
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    // Right-click to zoom out and set center position
                    complexPlane.zoomOut();
                    complexPlane.setCenter(sf::Mouse::getPosition(window));
                    complexPlane.updateRender();
                }
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Left-click to zoom in and set to center position
                    complexPlane.zoomIn();
                    complexPlane.setCenter(sf::Mouse::getPosition(window));
                    complexPlane.updateRender();
                }
                break;
            case sf::Event::MouseMoved:
                // Call setMouseLocation to store mouse coordinates
                complexPlane.setMouseLocation(sf::Mouse::getPosition(window));
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                break;
            default:
                break;
            }
        }

        // Update Scene segment
        complexPlane.updateRender();
        complexPlane.loadText(text);

        // Draw Scene segment
        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}
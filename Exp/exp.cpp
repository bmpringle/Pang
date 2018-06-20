
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    
    sf::VideoMode vmDesktop = sf::VideoMode::getDesktopMode	();
    const unsigned int gameWidth = vmDesktop.width;
    const unsigned int gameHeight = vmDesktop.height;
    const unsigned int bitspixel = vmDesktop.bitsPerPixel;
    bool PlayingGame = false;
    const float trianglespeed=gameHeight/2;
    sf::Vector2f trianglesize(gameWidth/32, gameHeight/20);
    sf::Time sleep = sf::seconds(0.05f);
    
     // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, bitspixel), "Experimental",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

       // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;
    //Load the BattleShip
        sf::CircleShape triangle(80, 3);
        triangle.setOutlineThickness(3);
        triangle.setOutlineColor(sf::Color::Black);
        triangle.setFillColor(sf::Color::White);
        sf::Vector2f pos = triangle.getPosition();
        sf::Vector2f ofs(80.0, 80.0);
        triangle.setOrigin(pos+ofs);
       
       
    // Initialize the pause message
    sf::Text TitleMessage;
    TitleMessage.setFont(font);
    TitleMessage.setCharacterSize(200);
    TitleMessage.setPosition(700.f, 900.f);
    TitleMessage.setFillColor(sf::Color::White);
    TitleMessage.setString("Welcome to Experimental!\n     Press SPACE to start!");

    sf::Clock clock;
    sf::Event event;

    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
            {
                if(!PlayingGame)
                {
                    PlayingGame = true;
                    triangle.setPosition(gameWidth/2, gameHeight/2);
                    clock.restart();
                }
            }
        }
        if(PlayingGame)
        {
             float deltaTime = clock.restart().asSeconds();
            //Move Spaceship
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
                (triangle.getPosition().y - trianglesize.y / 2 > 5.f))
            {
                triangle.move(0.f, -trianglespeed * deltaTime);
            }
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)  && (triangle.getPosition().y + trianglesize.y / 2 < gameHeight - 200.f))
            {
                triangle.move(0.f, trianglespeed * deltaTime);
              
            }
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (triangle.getPosition().x - trianglesize.x / 2 > 5.f))
            {
                triangle.move(-trianglespeed * deltaTime, 0.f);
            }
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && (triangle.getPosition().x + trianglesize.x / 2 < gameWidth - 200.f))
            {
                triangle.move(trianglespeed * deltaTime, 0.f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {
                
                triangle.rotate(10);
                sf::sleep(sleep);
            }
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                
                triangle.rotate(-10);
                sf::sleep(sleep);
            }

        }
             // Clear the window
        window.clear(sf::Color(0, 0, 0));
        if(PlayingGame){
             window.draw(triangle);
        }else{
        window.draw(TitleMessage);
        }
       
        // Display things on screen
        window.display();
    }
    return EXIT_SUCCESS;
}
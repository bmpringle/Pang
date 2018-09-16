
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
/// Entry point of application`a
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
    const float trianglespeed=gameHeight/100;
    sf::Vector2f trianglesize(gameWidth/32, gameHeight/20);
    sf::Time sleep = sf::seconds(0.005f);
    double diagonal = sqrt (pow(vmDesktop.width,2.0) + pow(vmDesktop.height, 2.0));
    float ballRadius = double(diagonal/100.0);

    
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

        // Create the missile
    sf::CircleShape missile;
    missile.setRadius(ballRadius - 3);
    missile.setOutlineThickness(3);
    missile.setOutlineColor(sf::Color::Black);
    missile.setFillColor(sf::Color::White);
    missile.setOrigin(ballRadius / 2, ballRadius / 2);
       
        
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

                  //Convert angle to radians
        double angleRADS = (3.1415926536/180)*(triangle.getRotation());

        //Set x and y
        double fory = trianglespeed*-cos(angleRADS);
        double forx = trianglespeed*sin(angleRADS);

        std::cout << "x variable = " << forx << " y variable = " << fory << " triangle speed variable = " << trianglespeed << " angle of ship = " << triangle.getRotation() << std::endl;

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
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                if(triangle.getPosition().y + trianglesize.y / 2 < gameHeight - 149.0f || (triangle.getRotation() >= 271 || triangle.getRotation() <= 89)) {

                    if(triangle.getPosition().y - trianglesize.y / 2 > 50.0f || (triangle.getRotation() <= 269 && triangle.getRotation() >= 91)){
                        
                        if(triangle.getPosition().x - trianglesize.x / 2 > 100.f || (triangle.getRotation() >= 1 && triangle.getRotation() <= 179))
                        {
                            if(triangle.getPosition().x + trianglesize.x / 2 < gameWidth - 70.f || (triangle.getRotation() >= 181))
                            {
                                
                                triangle.move(forx, fory);
                            
                            }
                            
                        }
                    
                        
                    }
                    
                }
                
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                if(triangle.getPosition().y + trianglesize.y / 2 < gameHeight - 149.0f || (triangle.getRotation() <= 269 && triangle.getRotation() >= 91)) {

                    if(triangle.getPosition().y - trianglesize.y / 2 > 50.0f || (triangle.getRotation() >= 271 || triangle.getRotation() <= 89)){
                        
                        if(triangle.getPosition().x - trianglesize.x / 2 > 100.f || (triangle.getRotation() <= 359 && triangle.getRotation() >= 181))
                        {
                            if(triangle.getPosition().x + trianglesize.x / 2 < gameWidth - 70.f || (triangle.getRotation() <= 179))
                            {
                                
                                triangle.move(-forx, -fory);
                            
                            }
                            
                        }
                    
                        
                    }
                    
                }
                
            }

             
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {
                
                triangle.rotate(5);
                sf::sleep(sleep);
            }
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                
                triangle.rotate(-5);
                sf::sleep(sleep);
            }

        }
             // Clear the window
        window.clear(sf::Color(0, 0, 0));
        if(PlayingGame){
             window.draw(triangle);
             if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                    missile.setPosition(triangle.getPosition());
                    window.draw(missile);
             }
        }else{
        window.draw(TitleMessage);
        }
       
        // Display things on screen
        window.display();
    }
    return EXIT_SUCCESS;
}
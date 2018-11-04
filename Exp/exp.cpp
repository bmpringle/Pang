
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

class Asteroid {
public:
    Asteroid(int sideLength, int nSides, int lineThickness, sf::Color outlineColor, sf::Color fillColor) :
    _circle(sideLength, nSides)
{
    _circle.setOutlineThickness(lineThickness);
    _circle.setOutlineColor(outlineColor);
    _circle.setFillColor(fillColor);
    _pos = _circle.getPosition();
    _circle.setOrigin(_pos + sf::Vector2f(sideLength, sideLength));
}
private:
    sf::CircleShape _circle;
    sf::Vector2f    _pos;
};

class AsteroidField {
public:
    AsteroidField(int sideLength, int nSides, int lineThickness, sf::Color outlineColor, sf::Color fillColor, uint numAsteroids)
    {
        for(int i=0; i<=numAsteroids; i++) {
            Asteroid asteroid(Asteroid(sideLength,  nSides,  lineThickness, outlineColor, fillColor));
            _vAsteroids.push_back(asteroid);
        }   
    }
private:
    std::vector<Asteroid> _vAsteroids;
};

class SpaceShip {
    const int sideLength = 80;
    const int nSides = 3;
    const int lineThickness = 3; 
    const sf::Color outlineColor = sf::Color::Black; 
    const sf::Color fillColor = sf::Color::White;
public:
    SpaceShip(sf::Vector2f size, float speed)
        : _circle(sideLength, nSides)
        , _size(size)
        , _speed(speed)
    {
        _circle.setOutlineThickness(lineThickness);
        _circle.setOutlineColor(outlineColor);
        _circle.setFillColor(fillColor);
        _pos = _circle.getPosition();
        _circle.setOrigin(_pos + sf::Vector2f(sideLength, sideLength));
    }

    void setPosition(float x, float y)
    {
        _circle.setPosition(x,y);
    }

    sf::Vector2f getPosition(void)
    {
        return _circle.getPosition();
    }

    void rotate(float angle) 
    {
        _circle.rotate(angle);
    }

    float getRotation(void)
    {
        return _circle.getRotation();
    }

    sf::Vector2f getSize(void)
    {
        return _size;
    }

    sf::Vector2f getVelocity(void)
    {
        //Convert angle to radians
        double angleRADS = (3.1415926536/180)*(_circle.getRotation());
        //Set x and y
        double y = _speed * -cos(angleRADS);
        double x = _speed * sin(angleRADS);
        return  sf::Vector2f(x,y);
    }

    void Move(bool bForward, sf::Vector2f screenSize)
    {
        sf::Vector2f vel = getVelocity();

        if(_circle.getPosition().y + _size.y / 2 < screenSize.y - 149.0f || (_circle.getRotation() >= 271 || _circle.getRotation() <= 89)) 
        {
            if(_circle.getPosition().y - _size.y / 2 > 50.0f || (_circle.getRotation() <= 269 && _circle.getRotation() >= 91))
            {
                if(_circle.getPosition().x - _size.x / 2 > 100.f || (_circle.getRotation() >= 1 && _circle.getRotation() <= 179))
                {
                    if(_circle.getPosition().x + _size.x / 2 < screenSize.x - 70.f || (_circle.getRotation() >= 181))
                    {
                        if (bForward) 
                        {
                            _circle.move(vel.x, vel.y);
                        } else 
                        {
                            _circle.move(-vel.x, -vel.y);
                        }
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) 
    {
        window.draw(_circle);
    }

private:
    sf::CircleShape _circle;
    sf::Vector2f    _pos;
    sf::Vector2f    _size; 
    float           _speed;
};

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    
    sf::VideoMode vmDesktop = sf::VideoMode::getDesktopMode	();
    const unsigned int gameWidth = vmDesktop.width;
    const unsigned int gameHeight = vmDesktop.height;
    const unsigned int bitspixel = vmDesktop.bitsPerPixel;
    bool PlayingGame = false;
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
    SpaceShip battleShip(sf::Vector2f(gameWidth/32, gameHeight/20), float(gameHeight)/100);

    // Create the missile
    sf::CircleShape missile;
    missile.setRadius(ballRadius - 8);
    missile.setOutlineThickness(3);
    missile.setOutlineColor(sf::Color::Black);
    missile.setFillColor(sf::Color::White);
    missile.setOrigin((ballRadius-8) / 2, (ballRadius-8) / 2);

    //Create the Asteroid Field
    AsteroidField asteroidField = AsteroidField(5, 8, 2, sf::Color::White, sf::Color::White, 32);
           
    // Initialize the pause message
    sf::Text TitleMessage;
    TitleMessage.setFont(font);
    TitleMessage.setCharacterSize(200);
    TitleMessage.setPosition(700.f, 900.f);
    TitleMessage.setFillColor(sf::Color::White);
    TitleMessage.setString("Welcome to Experimental!\n     Press SPACE to start!");

    sf::Clock clock;
    sf::Event event;
    float deltaTime=0;

    while (window.isOpen())
    {
        //std::cout << "x variable = " << forx << " y variable = " << fory << " triangle speed variable = " << trianglespeed << " angle of ship = " << triangle.getRotation() << std::endl;

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
                    battleShip.setPosition(gameWidth/2, gameHeight/2);
                    missile.setPosition(battleShip.getPosition());
                    clock.restart();
                    deltaTime = clock.restart().asSeconds();
                }
            }
        }

        // Clear the window
        window.clear(sf::Color(0, 0, 0));

        if(PlayingGame)
        {
            //Move Spaceship
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                battleShip.Move(true, sf::Vector2f(gameWidth,gameHeight));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                battleShip.Move(false, sf::Vector2f(gameWidth,gameHeight));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {                
                battleShip.rotate(5);
                sf::sleep(sleep);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                battleShip.rotate(-5);
                sf::sleep(sleep);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                sf::Vector2f vel = battleShip.getVelocity();
                missile.move(15*vel.x, 15*vel.y);    
            }

            battleShip.draw(window);
          
            if(missile.getPosition().y + battleShip.getSize().y / 2 < gameHeight - 149.0f || (battleShip.getRotation() >= 271 || battleShip.getRotation() <= 89)) 
            {
                if(missile.getPosition().y - battleShip.getSize().y / 2 > 50.0f || (battleShip.getRotation() <= 269 && battleShip.getRotation() >= 91))
                {    
                    if(missile.getPosition().x - battleShip.getSize().x / 2 > 100.f || (battleShip.getRotation() >= 1 && battleShip.getRotation() <= 179))
                    {
                        if(missile.getPosition().x + battleShip.getSize().x / 2 < gameWidth - 70.f || (battleShip.getRotation() >= 181))
                        {
                            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                            {
                                window.draw(missile);
                            }
                        } else 
                        {
                            missile.setPosition(battleShip.getPosition());
                        }  
                    } else 
                    {
                        missile.setPosition(battleShip.getPosition());
                    } 
                } else 
                {
                    missile.setPosition(battleShip.getPosition());
                }
            } else 
            {
                missile.setPosition(battleShip.getPosition());
            }
        } else // not playing game
        {
            window.draw(TitleMessage);
        }
        // Display things on screen
        window.display();
    }
    return EXIT_SUCCESS;
}
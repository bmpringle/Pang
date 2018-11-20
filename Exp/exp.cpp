////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <ctime>

////////////////////////////////////////////////////////////
/// Entry point of application`a
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////

class SpaceShip : public sf::CircleShape{
    static const int sideLength = 80;
    static const int nSides = 3;
    static const int lineThickness = 3;
    const sf::Color outlineColor = sf::Color::Black; 
    const sf::Color fillColor = sf::Color::White;
public:
    SpaceShip(sf::Vector2f size, float speed)
        : sf::CircleShape(sideLength, nSides)
        , _size(size)
        , _speed(speed)
    {
        setOutlineThickness(lineThickness);
        setOutlineColor(outlineColor);
        setFillColor(fillColor);
        _pos = getPosition();
        setOrigin(_pos + sf::Vector2f(sideLength, sideLength));
    }

    sf::Vector2f getVelocity(void)
    {
        //Convert angle to radians
        double angleRADS = (3.1415926536/180)*(getRotation());
        //Set x and y
        double y = _speed * -cos(angleRADS);
        double x = _speed * sin(angleRADS);
        return  sf::Vector2f(x,y);
    }

    void Move(bool bForward, sf::Vector2f screenSize)
    {
        sf::Vector2f vel = getVelocity();

        if(getPosition().y + _size.y / 2 < screenSize.y - 149.0f || (getRotation() >= 271 || getRotation() <= 89)) 
        {
            if(getPosition().y - _size.y / 2 > 50.0f || (getRotation() <= 269 && getRotation() >= 91))
            {
                if(getPosition().x - _size.x / 2 > 100.f || (getRotation() >= 1 && getRotation() <= 179))
                {
                    if(getPosition().x + _size.x / 2 < screenSize.x - 70.f || (getRotation() >= 181))
                    {
                        if (bForward) 
                        {
                            move(vel.x, vel.y);
                        } else 
                        {
                            move(-vel.x, -vel.y);
                        }
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) 
    {
        window.draw(*this);
    }

private:
    sf::Vector2f    _pos;
    sf::Vector2f    _size; 
    float           _speed;
};


class Asteroid : public sf::CircleShape {
public:
    Asteroid(int sideLength, int nSides, int lineThickness, sf::Color outlineColor, sf::Color fillColor)
    : sf::CircleShape(sideLength, nSides)
    {
        setOutlineThickness(lineThickness);
        setOutlineColor(outlineColor);
        setFillColor(fillColor);
        _pos = getPosition();
        setOrigin(_pos + sf::Vector2f(sideLength, sideLength));    
    }

    void Draw(sf::RenderWindow& window){
        window.draw(*this);
    }

private:
    sf::Vector2f    _pos;
};

class AsteroidField {
public:
    AsteroidField(int sideLength, int nSides, int lineThickness, sf::Color outlineColor, sf::Color fillColor, uint numAsteroids)
    {
        for(int i=0; i < numAsteroids; i++) {            
            Asteroid asteroid(Asteroid(sideLength,  nSides,  lineThickness, outlineColor, fillColor));
            _vAsteroids.push_back(asteroid);
            std::cout<< "Create Position =" << i << ": " << asteroid.getPosition().x << "," << asteroid.getPosition().y << std::endl;
        }   
    }

    void Draw(sf::RenderWindow& window, SpaceShip battleShip, int gameWidth, int gameHeight)
    {
        if(a==0) {
            for(int i=0; i < _vAsteroids.size(); i++) {
                Asteroid& asteroid = _vAsteroids[i];
                asteroid.setPosition(sf::Vector2f(gameWidth/2, gameHeight/2));
                std::cout<< "Astroid Field Initial Position =" << i << ": " << asteroid.getPosition().x << "," << asteroid.getPosition().y << std::endl;
                asteroid.Draw(window);
                a=1;
            }
        } else {
            for(int i=0; i < _vAsteroids.size(); i++) {
                Asteroid& asteroid = _vAsteroids[i];
                int r = rand() % 8;
                sf::Vector2f add(0, 0);
                sf::Vector2f one(0, 3);
                sf::Vector2f two(3, 3);
                sf::Vector2f three(3, 0);
                sf::Vector2f four(3, -3);
                sf::Vector2f five(0, -3);
                sf::Vector2f six(-3, -3);
                sf::Vector2f seven(-3, 0);
                sf::Vector2f eight(-3, 3);
                
                switch(r) {
                    case 0:
                    add = one;
                    break;
                    
                    case 1:
                    add = two;
                    break;
                    
                    case 2:
                    add = three;
                    break;
                    
                    case 3:
                    add = four;
                    break;

                    case 4:
                    add = five;
                    break;
                    
                    case 5:
                    add = six;
                    break;
                    
                    case 6:
                    add = seven;
                    break;
                    
                    case 7:
                    add = eight;
                    break;
                }
                asteroid.move(add);
                //std::cout<< "Update Position =" << i << ": " << asteroid.getPosition().x << "," << asteroid.getPosition().y << std::endl;

                asteroid.Draw(window);
            }
        }
    }
private:
    std::vector<Asteroid> _vAsteroids;
    int a=0;
};

class Missile : public sf::CircleShape {  
    public:
    Missile(int radius, sf::Vector2f size, float speed)
        : _speed(speed)
        , _size(size)
    {
        setRadius(radius);
        setOutlineThickness(3);
        setOutlineColor(sf::Color::Black);
        setFillColor(sf::Color::White);
        setOrigin((radius) / 2, (radius) / 2);
    }
 
    sf::Vector2f getSize(void)
    {
        return _size;
    }

    void Move(bool bForward, SpaceShip battleShip)
    {
        sf::Vector2f vel = battleShip.getVelocity();
        move(vel.x*15, vel.y*15);   
    }

    void Draw(sf::RenderWindow& window, sf::Vector2f screenSize, SpaceShip battleShip)
    {
        if(getPosition().y + getSize().y / 2 < screenSize.y - 149.0f) 
        {
            if(getPosition().y - getSize().y / 2 > 50.0f)
            {    
                if(getPosition().x - getSize().x / 2 > 100.f)
                {
                    if(getPosition().x + getSize().x / 2 < screenSize.x - 70.f)
                    {
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                        {
                            window.draw(*this);
                        }
                    } else 
                    {
                        setPosition(battleShip.getPosition());
                    }  
                } else 
                {
                    setPosition(battleShip.getPosition());
                } 
            } else 
            {
                setPosition(battleShip.getPosition());
            }
        } else 
        {
            setPosition(battleShip.getPosition());
        }
    }

private:
    float _speed;
    sf::Vector2f _size;
};


int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    
    sf::VideoMode vmDesktop = sf::VideoMode::getDesktopMode();
    const unsigned int gameWidth = vmDesktop.width;
    const unsigned int gameHeight = vmDesktop.height;
    const unsigned int bitspixel = vmDesktop.bitsPerPixel;
    bool Stopped = false;
    bool PlayingGame = false;
    sf::Time sleep = sf::seconds(0.005f);
    double diagonal = sqrt (pow(vmDesktop.width,2.0) + pow(vmDesktop.height, 2.0));
    float ballRadius = double(diagonal/100.0);

     // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, bitspixel), "Asteroids",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

       // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    //Load the BattleShip
    SpaceShip battleShip(sf::Vector2f(gameWidth/32, gameHeight/20), float(gameHeight)/100);

    //Load the Missile
    Missile missile(ballRadius - 8, sf::Vector2f(gameWidth/32, gameHeight/20), float(gameHeight)/100);
    
    //Create the Asteroid Field
    AsteroidField asteroidField = AsteroidField(80, 8, 2, sf::Color::White, sf::Color::White, 2);
           
    // Initialize the pause message
    sf::Text TitleMessage;
    TitleMessage.setFont(font);
    TitleMessage.setCharacterSize(200);
    TitleMessage.setPosition(700.f, 900.f);
    TitleMessage.setFillColor(sf::Color::White);
    TitleMessage.setString("Welcome to Asteroids!\n     Press T to start!");

    sf::Clock clockm;
    sf::Clock clock;
    sf::Event event;
    
    std::cout << "desktop: x=" << gameWidth << ", y=" << gameHeight << std::endl;
    while (window.isOpen())
    {
        //std::cout << " battleShip Velocity: x= " << battleShip.getVelocity().x;
        //std::cout << "y=" << battleShip.getVelocity().y;
        //std::cout << " battleShip angle = " << battleShip.getRotation() << std::endl;

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
            if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::T))
            {
                if(!PlayingGame)
                {
                    PlayingGame = true;
                    battleShip.setPosition(gameWidth/2, gameHeight/2);
                    missile.setPosition(battleShip.getPosition());
                    
                }
            }
        }

        // Clear the window
        window.clear(sf::Color(0, 0, 0));

        if(PlayingGame)
        {
            float deltaTime=4;
            
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
                if(deltaTime>3)
                missile.Move(true, battleShip);
                
                deltaTime=clock.restart().asSeconds();
            }

            // draw objects
            battleShip.draw(window);
            missile.Draw(window, sf::Vector2f(gameWidth,gameHeight), battleShip);
            asteroidField.Draw(window, battleShip, gameWidth, gameHeight);

        } else // not playing game
        {
            window.draw(TitleMessage);
        }

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
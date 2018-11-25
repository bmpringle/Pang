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

typedef enum {
    MOVEMODE_CLAMP      = 0,
    MOVEMODE_WRAP       = 1,
    MOVEMODE_COLLIDE    = 2
} E_MOVEMODE;

class MovableObject : public sf::CircleShape {
public:
    MovableObject(int radius, int nSides, float speed, 
                  E_MOVEMODE move_mode = MOVEMODE_CLAMP)
        : sf::CircleShape(radius, nSides)
        , _speed(speed)
        , _move_mode(move_mode)
    {
        setOrigin(sf::Vector2f(radius,radius)); // set the origin to the center of polygon
        dumpInit();
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

    void draw(sf::RenderWindow& window) 
    {
        window.draw(*this);
    }

    static void setBounds(int left, int top, int width, int height)
    {
        _bounds = sf::Rect<int>(left,top, width, height);
    }

    static const sf::Rect<int>& getBounds(void)
    {
        return _bounds;
    }

    typedef int32_t coord_t;
    void dumpMove(coord_t old_x, coord_t old_y, coord_t inc_x, coord_t inc_y,
            coord_t new_x, coord_t new_y)
    {
        std::cout << "old(" << old_x << ", " << old_y << "), ";
        std::cout << "inc(" << inc_x << ", " << inc_y << "), ";
        std::cout << "new(" << new_x << ", " << new_y << "), ";
        std::cout << "bnd(" << _bounds.width << ", " << _bounds.height << ")" << std::endl;
    }

    void dumpInit(void)
    {
        std::cout << "getPosition(" << getPosition().x << "," << getPosition().y << ")" << std::endl;
        std::cout << "getOrigin(" << getOrigin().x << "," << getOrigin().y << ")" << std::endl;
        std::cout << "getRadius(" << getRadius() << ")" << std::endl;
        std::cout << "bnd(" << _bounds.width << ", " << _bounds.height << ")" << std::endl;
    }

    double getRadiusX(void)
    {
        double angleRADS = (3.1415926536/180) * getRotation();
        return getRadius() * sin(angleRADS);
    }

    double getRadiusY(void)
    {
        double angleRADS = (3.1415926536/180) * getRotation();
        return getRadius() * -cos(angleRADS);
    }

    void move(sf::Vector2f ofs) {return move(ofs.x, ofs.y);}

    void move(float x, float y)
    {
        sf::Vector2f pos = getPosition();
        coord_t old_x = static_cast<coord_t>(pos.x);
        coord_t old_y = static_cast<coord_t>(pos.y);
        coord_t inc_x = static_cast<coord_t>(x);
        coord_t inc_y = static_cast<coord_t>(y);

        switch (_move_mode) {
            case MOVEMODE_CLAMP: {
                coord_t new_x = old_x + inc_x;
                coord_t new_y = old_y + inc_y;
                //dumpMove(old_x, old_y, inc_x, inc_y, new_x, new_y);
                if (_bounds.contains(new_x + getRadiusX(), new_y + getRadiusY())) {
                    sf::CircleShape::move(x, y);
                }
                break;
            }
            case MOVEMODE_WRAP: {
                coord_t new_x = (old_x + inc_x) >= 0 ? (old_x + inc_x) % _bounds.width : _bounds.width;
                coord_t new_y = (old_y + inc_y) >= 0 ? (old_y + inc_y) % _bounds.height : _bounds.height;
                //dumpMove(old_x, old_y, inc_x, inc_y, new_x, new_y);
                sf::CircleShape::setPosition(new_x,new_y);
                break;
            }
            case MOVEMODE_COLLIDE: {
                assert(0);
            }
            default: assert(0);
        }
    }

protected:
    float                   _speed;
    E_MOVEMODE              _move_mode;
    static sf::Rect<int>    _bounds;
};

sf::Rect<int> MovableObject::_bounds = sf::Rect<int>(0,0,0,0);

class SpaceShip : public MovableObject {
    static const int sideLength = 80;
    static const int nSides = 3;
    static const int lineThickness = 3;
    const sf::Color outlineColor = sf::Color::Black; 
    const sf::Color fillColor = sf::Color::White;
public:
    SpaceShip(float speed)
        : MovableObject(sideLength, nSides, speed,
                        MOVEMODE_WRAP)
    {
        setOutlineThickness(lineThickness);
        setOutlineColor(outlineColor);
        setFillColor(fillColor);
    }

    void move(bool bForward, sf::Vector2f screenSize)
    {
        sf::Vector2f vel = getVelocity();
        sf::Vector2f ofs = bForward ? vel : -vel;
        return MovableObject::move(ofs);
    }
};

class Asteroid : public MovableObject {
    static const int speed = 0;
    static const int lineThickness = 3;
    const sf::Color outlineColor = sf::Color::White; 
    const sf::Color fillColor = sf::Color::White;
public:
    Asteroid(int sideLength, int nSides)
        : MovableObject(sideLength, nSides, speed,
                    MOVEMODE_WRAP)
    {
        setOutlineThickness(lineThickness);
        setOutlineColor(outlineColor);
        setFillColor(fillColor);
    }
};

class AsteroidField {
public:
    AsteroidField(int sideLength, int nSides, uint numAsteroids, uint amplitude = 3)
    : _amplitude(amplitude)
    {
        for(int i=0; i < numAsteroids; i++) {            
            Asteroid asteroid(sideLength,  nSides);
            _vAsteroids.push_back(asteroid);
            std::cout<< "Create Position =" << i << ": " << asteroid.getPosition().x << "," << asteroid.getPosition().y << std::endl;
        }   
    }

    void draw(sf::RenderWindow& window, SpaceShip battleShip, int gameWidth, int gameHeight)
    {
        if(a==0) {
            for(int i=0; i < _vAsteroids.size(); i++) {
                Asteroid& asteroid = _vAsteroids[i];
                asteroid.setPosition(sf::Vector2f(gameWidth/2, gameHeight/2));
                std::cout<< "Astroid Field Initial Position =" << i << ": " << asteroid.getPosition().x << "," << asteroid.getPosition().y << std::endl;
                asteroid.draw(window);
                a=1;
            }
        } else {
            for(int i=0; i < _vAsteroids.size(); i++) {
                Asteroid& asteroid = _vAsteroids[i];
                sf::Vector2f add;
                switch(rand() % 8) {
                    case 0: add = sf::Vector2f(0,           _amplitude);  break;
                    case 1: add = sf::Vector2f(_amplitude,  _amplitude);  break;
                    case 2: add = sf::Vector2f(_amplitude,  0);           break;
                    case 3: add = sf::Vector2f(_amplitude,  -_amplitude); break;
                    case 4: add = sf::Vector2f(0,           -_amplitude); break;
                    case 5: add = sf::Vector2f(-_amplitude, -_amplitude); break;
                    case 6: add = sf::Vector2f(-_amplitude, 0);           break;
                    case 7: add = sf::Vector2f(-_amplitude, _amplitude);  break;
                    default: assert(0); break;
                }
                asteroid.move(add);
                //std::cout<< "Update Position =" << i << ": " << asteroid.getPosition().x << "," << asteroid.getPosition().y << std::endl;
                asteroid.draw(window);
            }
        }
    }
private:
    std::vector<Asteroid> _vAsteroids;
    int                   _amplitude;
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

    void move(bool bForward, SpaceShip battleShip)
    {
        sf::Vector2f vel = battleShip.getVelocity();
        sf::CircleShape::move(vel.x*15, vel.y*15);   
    }

    void draw(sf::RenderWindow& window, sf::Vector2f screenSize, SpaceShip battleShip)
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

    MovableObject::setBounds(0, 0, vmDesktop.width, vmDesktop.height);

    //Load the BattleShip
    SpaceShip battleShip(float(gameHeight)/100);

    //Load the Missile
    Missile missile(ballRadius - 8, sf::Vector2f(gameWidth/32, gameHeight/20), float(gameHeight)/100);
    //Create the Asteroid Field
    AsteroidField asteroidField = AsteroidField(80, 8, 2);
           
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Up) )
            {
                battleShip.move(true, sf::Vector2f(gameWidth,gameHeight));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Down) )
            {
                battleShip.move(false, sf::Vector2f(gameWidth,gameHeight));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Right) )
            {                
                battleShip.rotate(5);
                sf::sleep(sleep);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Left) )
            {
                battleShip.rotate(-5);
                sf::sleep(sleep);
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if(deltaTime>3)
                missile.move(true, battleShip);
                
                deltaTime=clock.restart().asSeconds();
            }

            // draw objects
            battleShip.draw(window);
            missile.draw(window, sf::Vector2f(gameWidth,gameHeight), battleShip);
            asteroidField.draw(window, battleShip, gameWidth, gameHeight);

        } else // not playing game
        {
            window.draw(TitleMessage);
        }

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
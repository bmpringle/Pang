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

    sf::Vector2f getPosition(){
    return _circle.getPosition();
    }

    void setPosition(int x, int y){
       _circle.setPosition(x, y); 
    }

    void setPosition(sf::Vector2f position ){
       _circle.setPosition(position); 
    }

    void Draw(sf::RenderWindow& window){
        window.draw(_circle);
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

    void Draw(sf::RenderWindow& window, uint numAsteroids, SpaceShip battleShip, int gameWidth, int gameHeight)
    {
        if(a==0) {
            for(int i=0; i<=numAsteroids; i++) {
                Asteroid asteroid = _vAsteroids[i];
                
                asteroid.Draw(window);
                a=1;
            }
        } else {
            for(int i=0; i<=numAsteroids; i++) {
                Asteroid asteroid = _vAsteroids[i];
                srand(((std::time(NULL)*3)/2)-5000+1234);
                int r = rand() % 8;
                sf::Vector2f add(0, 0);
                sf::Vector2f pos(gameWidth/2, gameHeight/2);
                sf::Vector2f one(0, 300);
                sf::Vector2f two(300, 300);
                sf::Vector2f three(300, 0);
                sf::Vector2f four(300, -300);
                sf::Vector2f five(0, -300);
                sf::Vector2f six(-300, -300);
                sf::Vector2f seven(-300, 0);
                sf::Vector2f eight(-300, 300);
                
                
                switch(r) {
                    case 1:
                    add = one;
                    break;
                    
                    case 2:
                    add = two;
                    break;
                    
                    case 3:
                    add = three;
                    break;
                    
                    case 4:
                    add = four;
                    break;
                    
                    case 5:
                    add = five;
                    break;
                    
                    case 6:
                    add = six;
                    break;
                    
                    case 7:
                    add = seven;
                    break;
                    
                    case 8:
                    add = eight;
                    break;
                }
                sf::Vector2f newPos(pos.x+add.x, pos.y+add.y);
                
                asteroid.setPosition(newPos);
                _vAsteroids.push_back(asteroid);
                asteroid.Draw(window);
                //std::cout << "Seed = " << time(NULL) << std::endl;
                //std::cout << "Random number = " << r << std::endl;
                std::cout<< "Initial Position =" << pos.x << ", " << pos.y << std::endl;
            }
        }
    }
private:
    std::vector<Asteroid> _vAsteroids;
    int a=0;
};

class Missile{  
    public:
    Missile(int radius, sf::Vector2f size, float speed)
        : _speed(speed)
        , _size(size)
    {
        _circle.setRadius(radius);
        _circle.setOutlineThickness(3);
        _circle.setOutlineColor(sf::Color::Black);
        _circle.setFillColor(sf::Color::White);
        _circle.setOrigin((radius) / 2, (radius) / 2);
    }
 
    void setPosition(float x, float y)
    {
        _circle.setPosition(x,y);
    }

    void setPosition(sf::Vector2f pos)
    {
        setPosition(pos.x,pos.y);
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

    void Move(bool bForward, SpaceShip battleShip)
    {
        sf::Vector2f vel = battleShip.getVelocity();
        _circle.move(vel.x*15, vel.y*15);   
    }

    void Draw(sf::RenderWindow& window, sf::Vector2f screenSize, SpaceShip battleShip)
    {
        if(_circle.getPosition().y + getSize().y / 2 < screenSize.y - 149.0f) 
        {
            if(_circle.getPosition().y - getSize().y / 2 > 50.0f)
            {    
                if(_circle.getPosition().x - getSize().x / 2 > 100.f)
                {
                    if(_circle.getPosition().x + getSize().x / 2 < screenSize.x - 70.f)
                    {
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                        {
                            window.draw(_circle);
                        }
                    } else 
                    {
                        _circle.setPosition(battleShip.getPosition());
                    }  
                } else 
                {
                    _circle.setPosition(battleShip.getPosition());
                } 
            } else 
            {
                _circle.setPosition(battleShip.getPosition());
            }
        } else 
        {
            _circle.setPosition(battleShip.getPosition());
        }
    }

private:
    sf::CircleShape _circle;
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
            asteroidField.Draw(window, 2, battleShip, gameWidth, gameHeight);

        } else // not playing game
        {
            window.draw(TitleMessage);
        }

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
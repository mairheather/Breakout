#include "Ball.h"
#include "GameManager.h" // avoid cicular dependencies

Ball::Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager)
    : _window(window), _velocity(velocity), _gameManager(gameManager),
    _timeWithPowerupEffect(0.f), _isFireBall(false), _isAlive(true), _direction({1,1})
{
    _sprite.setRadius(RADIUS);
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition(0, 300);

    _trailSprite.setRadius(RADIUS);
    _trailSprite.setFillColor(sf::Color::Cyan);
    _trailSprite.setPosition(0, 300);
}

Ball::~Ball()
{
}

void Ball::update(float dt)
{
    // check for powerup, tick down or correct
    if (_timeWithPowerupEffect > 0.f)
    {
        _timeWithPowerupEffect -= dt;
    }
    else
    {
        if (_velocity != VELOCITY)
            _velocity = VELOCITY;   // reset speed.
        else if (_isFireBall)
        {
            setFireBall(0);    // disable fireball
            _sprite.setFillColor(sf::Color::Cyan);  // back to normal colour.
        }

        else
        {
            _sprite.setRadius(RADIUS); // set radius of sprite to normal
            _sprite.setOrigin(sf::Vector2f(RADIUS, RADIUS));
            trailRadius = RADIUS;
        }        
    }

    // Fireball effect
    if (_isFireBall)
    {
        // Flickering effect
        int flicker = rand() % 50 + 205; // Random value between 205 and 255
        _sprite.setFillColor(sf::Color(flicker, flicker / 2, 0)); // Orange flickering color
    }

    // Update position with a subtle floating-point error
    _sprite.move(_direction * _velocity * dt);

    // check bounds and bounce
    sf::Vector2f position = _sprite.getPosition();
    sf::Vector2u windowDimensions = _window->getSize();

    // bounce on walls
    if ((position.x >= windowDimensions.x - 2 * RADIUS && _direction.x > 0) || (position.x <= 0 && _direction.x < 0))
    {
        _direction.x *= -1;
    }

    // bounce on ceiling
    if (position.y <= 0 && _direction.y < 0)
    {
        _direction.y *= -1;
    }

    // lose life bounce
    if (position.y > windowDimensions.y)
    {
        _sprite.setPosition(0, 300);
        _direction = { 1, 1 };
        _gameManager->loseLife();
    }

    // collision with paddle
    if (_sprite.getGlobalBounds().intersects(_gameManager->getPaddle()->getBounds()))
    {
        _direction.y *= -1; // Bounce vertically

        float paddlePositionProportion = (_sprite.getPosition().x - _gameManager->getPaddle()->getBounds().left) / _gameManager->getPaddle()->getBounds().width;
        _direction.x = paddlePositionProportion * 2.0f - 1.0f;

        // Adjust position to avoid getting stuck inside the paddle
        _sprite.setPosition(_sprite.getPosition().x, _gameManager->getPaddle()->getBounds().top - 2 * RADIUS);
    }

    // collision with bricks
    int collisionResponse = _gameManager->getBrickManager()->checkCollision(_sprite, _direction, _isFireBall);
    if (_isFireBall) return; // no collisisons when in fireBall mode.
    if (collisionResponse == 1)
    {
        _direction.x *= -1; // Bounce horizontally
    }
    else if (collisionResponse == 2)
    {
        _direction.y *= -1; // Bounce vertically
    }
    
    if (previousBallPositions.size() > 200) { previousBallPositions.pop_back(); }

    if (!_isFireBall)
    {
        previousBallPositions.push_front(_sprite.getPosition());
    }

    if (_isFireBall) { previousBallPositions.clear(); }
  
}

void Ball::render()
{
    if (!_isFireBall)
    {
        for (int i = 0; i < previousBallPositions.size(); i++)
        {
            float radiusPercentage = (static_cast<float>(previousBallPositions.size()) - i) / previousBallPositions.size();

            _trailSprite.setRadius(trailRadius * radiusPercentage);
            _trailSprite.setOrigin(sf::Vector2f(trailRadius * radiusPercentage, trailRadius * radiusPercentage));
            _trailSprite.setFillColor(sf::Color::White);
            _trailSprite.setPosition(previousBallPositions[i]);
            _window->draw(_trailSprite);
        }
    }

    _window->draw(_sprite);
}

void Ball::setVelocity(float coeff, float duration)
{
    _velocity = coeff * VELOCITY;
    _timeWithPowerupEffect = duration;
}

void Ball::setFireBall(float duration)
{
    if (duration) 
    {
        _isFireBall = true;
        _timeWithPowerupEffect = duration;        
        return;
    }
    _isFireBall = false;
    _timeWithPowerupEffect = 0.f;    
}

void Ball::increaseBallSize(float multiplier, float duration)
{
    _timeWithPowerupEffect = duration;
    _sprite.setRadius(RADIUS * multiplier);
    _sprite.setOrigin(sf::Vector2f(RADIUS * multiplier, RADIUS * multiplier));
    trailRadius = RADIUS * multiplier;
}

void Ball::decreaseBallSize(float divider, float duration)
{
    _timeWithPowerupEffect = duration;
    _sprite.setRadius(RADIUS / divider); 
    _sprite.setOrigin(sf::Vector2f(RADIUS / divider, RADIUS / divider));

    trailRadius = RADIUS / divider;
}

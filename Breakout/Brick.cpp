#include "Brick.h"

Brick::Brick(float x, float y, float width, float height)
    : _isDestroyed(false)
{
    _shape.setPosition(x, y);
    _shape.setSize(sf::Vector2f(width, height));
    _shape.setFillColor(sf::Color::Red);
}

void Brick::render(sf::RenderWindow& window)
{
    if (!_isDestroyed) {
        window.draw(_shape);
    }
}

sf::FloatRect Brick::getBounds() const
{
    return _shape.getGlobalBounds();
}

int Brick::getHealth()
{
    return health;
}

void Brick::decreaseHealth(int damage)
{
    if (hitTimer >= collisionCooldown)
    {
        health -= damage;
    }
}

void Brick::incrementTimer(float dt)
{
    hitTimer += dt;
}

void Brick::resetTimer()
{
    hitTimer = 0;
}

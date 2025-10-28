#pragma once
#include <SFML/Graphics.hpp>

class Brick {
public:
    Brick(float x, float y, float width, float height);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;

    int getHealth();
    void decreaseHealth(int damage);

    void incrementTimer(float dt);
    void resetTimer();

private:
    sf::RectangleShape _shape;
    bool _isDestroyed;
    int health = 3;

    float hitTimer = 0;
    float collisionCooldown = 0.01;

};
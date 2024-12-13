#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball
{
public:
    sf::Sprite sprite;
    float dx, dy, x, y;

    Ball(sf::Texture &texture);
    void move();
    void checkCollision(int width, int height);
};

Ball::Ball(sf::Texture &texture) : dx(6), dy(5), x(300), y(300)
{
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void Ball::move()
{
    x += dx;
    y += dy;
    sprite.setPosition(x, y);
}

void Ball::checkCollision(int width, int height)
{
    if (x < 0 || x > width)
        dx = -dx;
    if (y < 0 || y > height)
        dy = -dy;
}
#endif

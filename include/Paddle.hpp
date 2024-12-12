#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

class Paddle {
public:
    sf::Sprite sprite;

    Paddle(sf::Texture& texture);
    void move(float dx);
};
Paddle::Paddle(sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setPosition(300, 440);
}

void Paddle::move(float dx) {
    sprite.move(dx, 0);
}
#endif

#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>

class Block
{
public:
    sf::Sprite sprite;

    Block(sf::Texture &texture, float x, float y);
};

Block::Block(sf::Texture &texture, float x, float y)
{
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

#endif

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <Ball.hpp>
#include <Block.hpp>
#include <Paddle.hpp>

class Game
{
private:
    sf::RenderWindow app;
    sf::Sprite background;
    std::vector<Block> blocks;
    const Ball ball;
    const Paddle paddle;

public:
    Game();
    void run();
    void handleInput();
    void update();
    void render();
};

Game::Game(): app(sf::VideoMode(520, 450), "Arkanoid!"), ball(sf::Texture()), paddle(sf::Texture())
{

    app.setFramerateLimit(60);

    sf::Texture blockTexture, backgroundTexture, ballTexture, paddleTexture;
    blockTexture.loadFromFile("images/block01.png");
    backgroundTexture.loadFromFile("images/background.jpg");
    ballTexture.loadFromFile("images/ball.png");
    paddleTexture.loadFromFile("images/paddle.png");

    background.setTexture(backgroundTexture);
    ball = Ball(ballTexture);
    paddle = Paddle(paddleTexture);

    for (int i = 1; i <= 10; ++i)
        for (int j = 1; j <= 10; ++j)
            blocks.emplace_back(blockTexture, i * 43, j * 20);
}

void Game::run()
{
    while (app.isOpen())
    {
        handleInput();
        update();
        render();
    }
}
#endif

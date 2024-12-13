#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <ctime>
#include <vector>
#include <SFML/Audio.hpp>

using namespace sf;

// Clase para manejar los bloques
class Block {
private:
    Sprite sprite;

public:
    Block(Texture& texture, float x, float y) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    FloatRect getBounds() {
        return sprite.getGlobalBounds();
    }
};

// Clase para manejar la bola
class Ball {
private:
    Sprite sprite;
    Vector2f velocity;

public:
    Ball(Texture& texture) {
        sprite.setTexture(texture);
        reset();
    }

    void move() {
        sprite.move(velocity);
        if (sprite.getPosition().x <= 0 || sprite.getPosition().x + sprite.getGlobalBounds().width >= 520)
            velocity.x *= -1;
        if (sprite.getPosition().y <= 0)
            velocity.y *= -1;
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    FloatRect getBounds() {
        return sprite.getGlobalBounds();
    }

    void reverseY() {
        velocity.y *= -1;
    }

    void reset() {
        sprite.setPosition(260, 450);
        velocity = {5, -5};
    }

    Vector2f getPosition() {
        return sprite.getPosition();
    }
};

// Clase para manejar la pala
class Paddle {
private:
    Sprite sprite;

public:
    Paddle(Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(210, 500);
    }

    void move(float dx) {
        sprite.move(dx, 0);
        if (sprite.getPosition().x < 0)
            sprite.setPosition(0, sprite.getPosition().y);
        if (sprite.getPosition().x + sprite.getGlobalBounds().width > 520)
            sprite.setPosition(520 - sprite.getGlobalBounds().width, sprite.getPosition().y);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    FloatRect getBounds() {
        return sprite.getGlobalBounds();
    }

    void reset() {
        sprite.setPosition(210, 500);
    }
};

// Clase principal que gestiona el juego
class Game {
private:
    enum GameState { Start, Playing, GameOver }; // Estados del juego
    GameState gameState;

    RenderWindow window;
    Texture blockTexture, bgTexture, ballTexture, paddleTexture, startTexture, gameOverTexture;
    Font font;
    Text blockText, livesText;
    std::vector<Block> blocks;
    Ball* ball;
    Paddle* paddle;
    int blocksDestroyed;
    int lives;
    bool ballAttached;

    sf::Music music;

public:
    Game() 
        : window(VideoMode(520, 550), "Arkanoid!"), gameState(Start), blocksDestroyed(0), lives(3), ballAttached(true) {
        window.setFramerateLimit(60);

        // Cargar texturas y fuentes
        blockTexture.loadFromFile("assets/images/block01.png");
        bgTexture.loadFromFile("assets/images/Background.PNG");
        ballTexture.loadFromFile("assets/images/ball.png");
        paddleTexture.loadFromFile("assets/images/paddle.png");
        startTexture.loadFromFile("assets/images/Inicio.png");
        gameOverTexture.loadFromFile("assets/images/fin.png");
        font.loadFromFile("assets/fonts/Minecraft.ttf");

        // Configurar texto
        blockText.setFont(font);
        blockText.setCharacterSize(20);
        blockText.setFillColor(Color::White);
        blockText.setPosition(10, 500);

        livesText.setFont(font);
        livesText.setCharacterSize(20);
        livesText.setFillColor(Color::White);
        livesText.setPosition(400, 500);

        // Inicializar objetos
        ball = new Ball(ballTexture);
        paddle = new Paddle(paddleTexture);

        // Crear bloques
        resetBlocks();

        // Inicializar música
        if (!music.openFromFile("assets/music/musica.ogg")) {
            throw std::runtime_error("No se pudo cargar la música.");
        }
        music.setLoop(true);
        music.play();
    }

    ~Game() {
        delete ball;
        delete paddle;
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            if (gameState == Playing) {
                update();
            }
            render();
        }
    }

private:
    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (gameState == Start) {
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                gameState = Playing;
            }
        } else if (gameState == GameOver) {
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                resetGame();
                gameState = Start;
            }
        } else if (gameState == Playing) {
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                paddle->move(-7);
            }

            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                paddle->move(7);
            }

            if (Keyboard::isKeyPressed(Keyboard::Space) && ballAttached) {
                ballAttached = false;
            }
        }
    }

    void update() {
        if (!ballAttached) {
            ball->move();
        }

        if (ball->getBounds().intersects(paddle->getBounds())) {
            ball->reverseY();
        }

        for (auto it = blocks.begin(); it != blocks.end(); ++it) {
            if (ball->getBounds().intersects(it->getBounds())) {
                blocks.erase(it);
                ball->reverseY();
                blocksDestroyed++;
                break;
            }
        }

        if (ball->getPosition().y > 550) {
            ball->reset();
            ballAttached = true;
            lives--;
            if (lives == 0) {
                gameState = GameOver;
            }
        }
    }

    void render() {
        window.clear();
        Sprite background(bgTexture);
        window.draw(background);

        if (gameState == Start) {
            Sprite startScreen(startTexture);
            window.draw(startScreen);
        } else if (gameState == GameOver) {
            Sprite gameOverScreen(gameOverTexture);
            window.draw(gameOverScreen);
        } else if (gameState == Playing) {
            for (auto& block : blocks) {
                block.draw(window);
            }

            ball->draw(window);
            paddle->draw(window);

            blockText.setString("Blocks: " + std::to_string(blocksDestroyed));
            
            livesText.setString("Lives: " + std::to_string(lives));

            window.draw(blockText);
            window.draw(livesText);
        }

        window.display();
    }

    void resetBlocks() {
        blocks.clear();
        for (int i = 1; i <= 10; i++) {
            for (int j = 1; j <= 10; j++) {
                blocks.emplace_back(blockTexture, i * 43, j * 20);
            }
        }

        for (int j = 1; j <= 10; j++) {
            blocks.emplace_back(blockTexture, 0, j * 20);
            blocks.emplace_back(blockTexture, 520 - 47, j * 20);
        }
    }

    void resetGame() {
        blocksDestroyed = 0;
        lives = 3;
        ballAttached = true;
        ball->reset();
        paddle->reset();
        resetBlocks();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}

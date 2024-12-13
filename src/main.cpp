#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <ctime>
#include <vector>

using namespace sf;

// Clase que representa la bola
class Ball {
public:
    Sprite sprite;
    float dx, dy;

    Ball(const Texture& texture) : dx(5), dy(4) {
        sprite.setTexture(texture);
        sprite.setPosition(300, 300);
    }

    void attachToPaddle(const Sprite& paddle) {
        sprite.setPosition(
            paddle.getPosition().x + paddle.getGlobalBounds().width / 2 - sprite.getGlobalBounds().width / 2,
            paddle.getPosition().y - sprite.getGlobalBounds().height
        );
    }

    void moveBall(bool& ballAttached, int& lives, bool& resetPosition, float screenWidth, float screenHeight) {
        sprite.move(dx, dy);
        Vector2f pos = sprite.getPosition();

        if (pos.x < 0 || pos.x > screenWidth - sprite.getGlobalBounds().width) dx = -dx;
        if (pos.y < 0) dy = -dy;

        if (pos.y > screenHeight) {
            lives--;
            ballAttached = true;
            resetPosition = true;
        }
    }

    FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
};

// Clase que representa la paleta
class Paddle {
public:
    Sprite sprite;

    Paddle(const Texture& texture) {
        sprite.setTexture(texture);
        sprite.setPosition(300, 460);
    }

    void moveLeft(float speed) {
        sprite.move(-speed, 0);
        if (sprite.getPosition().x < 0) sprite.setPosition(0, sprite.getPosition().y);
    }

    void moveRight(float speed, float screenWidth) {
        sprite.move(speed, 0);
        if (sprite.getPosition().x + sprite.getGlobalBounds().width > screenWidth) {
            sprite.setPosition(screenWidth - sprite.getGlobalBounds().width, sprite.getPosition().y);
        }
    }

    FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
};

// Clase que representa los bloques
class Block {
public:
    Sprite sprite;
    bool destroyed = false;

    Block(const Texture& texture, float x, float y) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
    }

    FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

    void destroy() {
        destroyed = true;
        sprite.setPosition(-100, -100); // Fuera de la pantalla
    }
};

// Clase principal que gestiona el juego
class Game {
private:
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

public:
    Game() : window(VideoMode(520, 550), "Arkanoid!"), blocksDestroyed(0), lives(3), ballAttached(true) {
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

    ~Game() {
        delete ball;
        delete paddle;
    }

    void resetGame() {
        blocksDestroyed = 0;
        lives = 3;
        ballAttached = true;

        // Reset bloques
        for (int i = 0; i < blocks.size(); i++) {
            blocks[i].destroyed = false;
            blocks[i].sprite.setPosition((i % 10 + 1) * 43, (i / 10 + 1) * 20);
        }

        ball->sprite.setPosition(300, 300);
        paddle->sprite.setPosition(300, 460);
    }

    void run() {
        showStartScreen();
        while (window.isOpen()) {
            handleEvents();

            if (ballAttached) {
                ball->attachToPaddle(paddle->sprite);
            } else {
                ball->moveBall(ballAttached, lives, ballAttached, 520, 550);
                handleCollisions();
            }

            if (lives <= 0) {
                showGameOverScreen();
                resetGame();
                continue;
            }

            updateUI();
            render();
        }
    }

    void showStartScreen() {
        Sprite startSprite(startTexture);
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) window.close();
                if (event.type == Event::KeyPressed || event.type == Event::MouseButtonPressed) return;
            }

            window.clear();
            window.draw(startSprite);
            window.display();
        }
    }

    void showGameOverScreen() {
        Sprite gameOverSprite(gameOverTexture);
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) window.close();
                if (event.type == Event::KeyPressed) return;
            }

            window.clear();
            window.draw(gameOverSprite);
            window.display();
        }
    }

    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) paddle->moveRight(8, 520);
        if (Keyboard::isKeyPressed(Keyboard::Left)) paddle->moveLeft(8);

        // Liberar la bola al presionar la barra espaciadora
        if (Keyboard::isKeyPressed(Keyboard::Space) && ballAttached) {
            ballAttached = false;
        }
    }

    void handleCollisions() {
        // Colisión bola con bloques
        for (auto& block : blocks) {
            if (!block.destroyed && ball->getBounds().intersects(block.getBounds())) {
                block.destroy();
                blocksDestroyed++;
                ball->dy = -ball->dy;
            }
        }

        // Colisión bola con la paleta
        if (ball->getBounds().intersects(paddle->getBounds())) {
            ball->dy = -(rand() % 5 + 2);
        }
    }

    void updateUI() {
        blockText.setString("Blocks destroyed: " + std::to_string(blocksDestroyed) + 
                            "\nBlocks left: " + std::to_string(blocks.size() - blocksDestroyed));
        livesText.setString("Lives: " + std::to_string(lives));
    }

    void render() {
        window.clear();

        // Fondo
        Sprite background(bgTexture);
        window.draw(background);

        // Bloques
        for (auto& block : blocks) {
            if (!block.destroyed) window.draw(block.sprite);
        }

        // Bola y paleta
        window.draw(ball->sprite);
        window.draw(paddle->sprite);

        // Texto
        window.draw(blockText);
        window.draw(livesText);

        window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}

#include <SFML/Graphics.hpp> 
#include <SFML/Window.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <time.h>

using namespace sf;

void resetGame(Sprite block[], int& n, Sprite& sBall, Sprite& sPaddle, float& x, float& y, float& dx, float& dy, bool& ballAttached, int& blocksDestroyed)
{
    n = 0;
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            block[n].setPosition(i * 43, j * 20);
            n++;
        }

    for (int j = 1; j <= 10; j++) {
        block[n].setPosition(0, j * 20);
        n++;
    }

    for (int j = 1; j <= 10; j++) {
        block[n].setPosition(520 - 47, j * 20);
        n++;
    }

    sPaddle.setPosition(300, 460);
    x = 300;
    y = 300;
    dx = 5;
    dy = 4;
    ballAttached = true;
    blocksDestroyed = 0; // Reiniciar el contador de bloques destruidos
}

int main()
{
    srand(time(0));

    // Crear ventana
    RenderWindow app(VideoMode(520, 550), "Arkanoid!");
    app.setFramerateLimit(60);

    // Cargar texturas
    Texture t1, t2, t3, t4, tInicio, tGameOver;
    t1.loadFromFile("assets/images/block01.png");
    t2.loadFromFile("assets/images/Background.PNG");
    t3.loadFromFile("assets/images/ball.png");
    t4.loadFromFile("assets/images/paddle.png");
    tInicio.loadFromFile("assets/images/Inicio.png");
    tGameOver.loadFromFile("assets/images/fin.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    Sprite sInicio(tInicio), sGameOver(tGameOver);

    sPaddle.setPosition(300, 460);

    Sprite block[1020];
    int n = 0;

    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            block[n].setTexture(t1);
            block[n].setPosition(i * 43, j * 20);
            n++;
        }

    for (int j = 1; j <= 10; j++) {
        block[n].setTexture(t1);
        block[n].setPosition(0, j * 20);
        n++;
    }

    for (int j = 1; j <= 10; j++) {
        block[n].setTexture(t1);
        block[n].setPosition(520 - 47, j * 20);
        n++;
    }

    float dx = 5, dy = 4;
    float x = 300, y = 300;

    bool ballAttached = true;

    // Contador de bloques
    int blocksDestroyed = 0;

    // Fuente para mostrar el contador
    Font font;
    font.loadFromFile("assets/fonts/Minecraft.ttf");

    Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(Color::White);
    text.setPosition(10, 500);

    // Pantalla de inicio
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter)
                goto START_GAME;
            if (e.type == Event::MouseButtonPressed)
                goto START_GAME;
        }

        app.clear();
        app.draw(sInicio);
        app.display();
    }

START_GAME:

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        if (ballAttached) {
            x = sPaddle.getPosition().x + sPaddle.getGlobalBounds().width / 2 - sBall.getGlobalBounds().width / 2;
            y = sPaddle.getPosition().y - sBall.getGlobalBounds().height;

            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                ballAttached = false;
            }
        } else {
            x += dx;
            for (int i = 0; i < n; i++)
                if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
                {
                    block[i].setPosition(-100, 0);
                    dx = -dx;
                    blocksDestroyed++; // Incrementar contador al romper un bloque
                }

            y += dy;
            for (int i = 0; i < n; i++)
                if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
                {
                    block[i].setPosition(-100, 0);
                    dy = -dy;
                    blocksDestroyed++; // Incrementar contador al romper un bloque
                }

            if (x < 0 || x > 520) dx = -dx;
            if (y < 0) dy = -dy;

            if (y > 550) {
                goto GAME_OVER;
            }

            if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds()))
                dy = -(rand() % 5 + 2);
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) sPaddle.move(8, 0);
        if (Keyboard::isKeyPressed(Keyboard::Left)) sPaddle.move(-8, 0);

        sBall.setPosition(x, y);

        // Actualizar texto del contador
        text.setString("Blocks destroyed: " + std::to_string(blocksDestroyed) + "\nBlocks left: " + std::to_string(n - blocksDestroyed));

        app.clear();
        app.draw(sBackground);
        app.draw(sBall);
        app.draw(sPaddle);

        for (int i = 0; i < n; i++)
            app.draw(block[i]);

        app.draw(text);
        app.display();
    }

GAME_OVER:

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter)
            {
                resetGame(block, n, sBall, sPaddle, x, y, dx, dy, ballAttached, blocksDestroyed);
                goto START_GAME;
            }
        }

        app.clear();
        app.draw(sGameOver);
        app.display();
    }

    return 0;
}

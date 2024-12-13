#include <SFML/Graphics.hpp> 
#include <SFML/Window.hpp>
#include <time.h>

using namespace sf;

int main()
{
    srand(time(0));

    // Crear ventana
    RenderWindow app(VideoMode(520, 550), "Arkanoid!");
    app.setFramerateLimit(60);

    // Cargar texturas
    Texture t1, t2, t3, t4, tInicio;
    t1.loadFromFile("assets/images/block01.png");
    t2.loadFromFile("assets/images/bg5.PNG");
    t3.loadFromFile("assets/images/ball.png");
    t4.loadFromFile("assets/images/paddle.png");
    tInicio.loadFromFile("assets/images/Inicio.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    Sprite sInicio(tInicio);

    sPaddle.setPosition(300, 460);

    Sprite block[1000];
    int n = 0;

    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            block[n].setTexture(t1);
            block[n].setPosition(i * 43, j * 20);
            n++;
        }

    float dx = 5, dy = 4;
    float x = 300, y = 300;

    bool ballAttached = true; // Bandera para mantener la pelota en el paddle

    // Pantalla de inicio
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            // Si se presiona Enter o Click, salir del bucle de inicio
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter)
                goto START_GAME;
            if (e.type == Event::MouseButtonPressed)
                goto START_GAME;
        }

        // Dibujar pantalla de inicio
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
            // Mantener la pelota sobre el paddle
            x = sPaddle.getPosition().x + sPaddle.getGlobalBounds().width / 2 - sBall.getGlobalBounds().width / 2;
            y = sPaddle.getPosition().y - sBall.getGlobalBounds().height;

            // Soltar la pelota al presionar la barra espaciadora
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
                }

            y += dy;
            for (int i = 0; i < n; i++)
                if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
                {
                    block[i].setPosition(-100, 0);
                    dy = -dy;
                }

            if (x < 0 || x > 520) dx = -dx; // Rebote en las paredes izquierda y derecha
            if (y < 0) dy = -dy;           // Rebote en la parte superior

            // Si la pelota toca la parte inferior de la ventana
            if (y > 550) {
                app.close(); // Cerrar el juego
            }

            if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds()))
                dy = -(rand() % 5 + 2);
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) sPaddle.move(8, 0);
        if (Keyboard::isKeyPressed(Keyboard::Left)) sPaddle.move(-8, 0);

        sBall.setPosition(x, y);

        app.clear();
        app.draw(sBackground);
        app.draw(sBall);
        app.draw(sPaddle);

        for (int i = 0; i < n; i++)
            app.draw(block[i]);

        app.display();
    }

    return 0;
}

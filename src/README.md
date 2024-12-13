Este proyecto es una implementación del clásico juego Arkanoid, desarrollado en C++ utilizando la biblioteca gráfica SFML. El juego cuenta con los siguientes elementos principales:

	* Bola (Ball): Se mueve por la pantalla, rebota en las paredes y destruye bloques. Si la bola toca el borde inferior de la pantalla, pierdes una vida.
	* Paleta (Paddle): Controlada por el jugador para evitar que la bola caiga y para redirigir su trayectoria.
	* Bloques (Block): Los objetivos que deben destruirse para acumular puntos.
	* Interfaz gráfica: El juego muestra un fondo, texto informativo sobre el progreso y las vidas restantes.
	El juego incluye una pantalla de inicio, una mecánica para reiniciar el juego al perder todas las vidas y un mensaje de "Game Over".


Cómo Jugar
	Objetivo
		Destruye todos los bloques moviendo la paleta para redirigir la bola y evitar que caiga al borde inferior de la pantalla. Mantén el control mientras acumulas puntos.

	Controles
		* ← Flecha Izquierda: Mueve la paleta hacia la izquierda.
		* → Flecha Derecha: Mueve la paleta hacia la derecha.
		* Espacio: Suelta la bola de la paleta y comienza el juego.

Instrucciones
	1. Al iniciar el juego, se mostrará una pantalla de inicio. Presiona cualquier tecla o clic para empezar.
	2. La bola comenzará adherida a la paleta. Usa la barra espaciadora para liberarla.
	3. Mueve la paleta con las flechas izquierda y derecha para mantener la bola en juego.
	4. Destruye todos los bloques para ganar puntos.
	5. Pierdes una vida si la bola cae al borde inferior. El juego termina cuando las vidas llegan a 0.
	6. Si pierdes, se mostrará una pantalla de "Game Over". Podrás reiniciar el juego desde el comienzo.
Estructura del Proyecto

/assets
├── images
│   ├── block01.png         # Imagen de los bloques
│   ├── Background.PNG      # Fondo del juego
│   ├── ball.png            # Imagen de la bola
│   ├── paddle.png          # Imagen de la paleta
│   ├── Inicio.png          # Imagen de la pantalla inicial
│   ├── fin.png             # Imagen de la pantalla de Game Over
├── fonts
│   ├── Minecraft.ttf       # Fuente utilizada en el texto
/src
├── main.cpp                # Código principal del juego

Requisitos del Sistema
	* C++ (compilador compatible con C++17 o superior).
	* SFML 2.5+ (biblioteca gráfica).
	* Un sistema operativo compatible (Windows, Linux o macOS).

Instrucciones de Instalación
	1. Instalar SFML:
		* Descarga e instala SFML desde sfml-dev.org.
		* Configura tu entorno de desarrollo para incluir los directorios de encabezados y bibliotecas de SFML.
	2. Clonar el repositorio:


		git clone https://github.com/tu-usuario/arkanoid.git
		cd arkanoid
	
	3. Compilar el proyecto: Utiliza tu compilador preferido (por ejemplo, g++) para compilar el código:
		g++ -o Arkanoid main.cpp -lsfml-graphics -lsfml-window -lsfml-system
	
	4, Ejecutar el juego:
		./Arkanoid

Características Adicionales
	* Colisiones: La bola detecta colisiones con la paleta, las paredes y los bloques.
	* Pantalla de inicio y Game Over: Visualizaciones gráficas para mejorar la experiencia del usuario.
	* Vidas y puntaje: El juego rastrea la cantidad de bloques destruidos y las vidas restantes.

Capturas de Pantalla
	Pantalla de Inicio
		(../assets/images/pantalla_de_inicio.png)

	Durante el Juego
	assets/images/juego.png

	Game Over
		(../assets/images/Game_Over.png)
Licencia
    Este proyecto está bajo la Licencia MIT. Siéntete libre de usar, modificar y distribuir este código según sea necesario.
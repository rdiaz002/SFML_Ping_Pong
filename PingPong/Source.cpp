#include <SFML/Graphics.hpp>
#include <iostream>
#include "MainGame.h"

using namespace std;

int main()
{
	const int WIDTH = 1600, HEIGHT = 900;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pong");
	MainGame game(window);
	game.startScreenLoop();
	

	return 0;
}




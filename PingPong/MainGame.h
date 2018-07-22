#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
class MainGame
{
public:
	MainGame(sf::RenderWindow & main_window);
	void startScreenLoop();

private:
	sf::RenderWindow * main_window;
	sf::RectangleShape paddle;
	sf::RectangleShape paddle2;
	sf::CircleShape ball;
	sf::RectangleShape lWall, rWall, uWall, dWall;
	sf::Vector2f p1movement, p2movement;
	sf::Vector2f ballMovement;
	sf::Font myfont;
	sf::Time TIMEOUT;

	//different factors for speeds of paddles and ball
	float x = .1f, y = .1f;
	float speed = 2.0f;

	float ballSpeed = 2.0f;
	float ballXDirection = ballSpeed, ballYDirection = ballSpeed;

	bool p1up = false, p1down = false;
	bool p2up = false, p2down = false;

	bool start_screen = true;
	const int WIDTH = 1600, HEIGHT = 900;
	std::string ip_addr;
	sf::IpAddress source;
	unsigned short source_port, dest_port;
	sf::IpAddress dest;
	void gameLoop();
	void hostGameLoop();
	void joinGameLoop();
	void textInputWindow();
	void update();

	void showErrorMessage();

	std::string getHostData();
	std::string getJoinData();
	float * parseData(std::string data);
	float * receiveData(sf::UdpSocket & soc);
	void sendData(sf::UdpSocket & soc, const std::string & data);
	sf::RectangleShape createRectangle(int posX, int posY, int sizeX, int sizeY);
	bool checkIntersection(sf::Shape& one, sf::Shape& two);
};


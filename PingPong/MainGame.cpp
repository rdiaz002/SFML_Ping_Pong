#include "MainGame.h"
#include <iostream>
#include <SFML/Network.hpp>
#include <string>

MainGame::MainGame(sf::RenderWindow & main_window)
{
	this->main_window = &main_window;
	myfont.loadFromFile("arial.ttf");
	TIMEOUT = sf::Time(sf::seconds(10));
	ball = sf::CircleShape(10.0f);
	ball.setFillColor(sf::Color::Yellow);
	ball.setPosition(WIDTH / 2, HEIGHT / 2);

	//paddles initialization
	paddle = createRectangle(20, HEIGHT / 2, 5, 100);
	paddle2 = createRectangle(WIDTH - 20, HEIGHT / 2, 5, 100);
	paddle.setOutlineThickness(5);
	paddle2.setOutlineThickness(5);

	//edges creation
	lWall = createRectangle(0, 0, 0, HEIGHT);
	rWall = createRectangle(WIDTH, 0, 0, HEIGHT);
	uWall = createRectangle(0, 0, WIDTH, 0);
	dWall = createRectangle(0, HEIGHT, WIDTH, 0);
}


void MainGame::startScreenLoop() {
	//Welcome Message 
	sf::Text welcome_message;
	welcome_message.setString("Hello Welcome To Pong!");
	welcome_message.setStyle(sf::Text::Bold | sf::Text::Underlined);

	//Standard Font
	
	welcome_message.setFont(myfont);
	welcome_message.setCharacterSize(25);
	welcome_message.setFillColor(sf::Color::Green);
	welcome_message.setPosition(sf::Vector2f(10.0f, 10.0f));

	//Host Button Texture
	sf::Texture texture;
	texture.loadFromFile("Host_button.png");
	sf::Sprite host_button(texture);
	host_button.setPosition(main_window->getSize().x / 2, main_window->getSize().y / 2);

	//Join Button Texture
	sf::Texture texture2;
	texture2.loadFromFile("Join_button.png");
	sf::Sprite join_button(texture2);
	join_button.setPosition(main_window->getSize().x / 2, (main_window->getSize().y / 2) + host_button.getGlobalBounds().height + 20);

	sf::Texture texture3;
	texture3.loadFromFile("Local_button.png");
	sf::Sprite local_button(texture3);
	local_button.setPosition(main_window->getSize().x / 2, join_button.getPosition().y + join_button.getGlobalBounds().height + 20);

	main_window->clear();
	main_window->draw(welcome_message);
	main_window->draw(host_button);
	main_window->draw(join_button);
	main_window->draw(local_button);
	main_window->display();

	while (main_window->isOpen()) {

		if (host_button.getGlobalBounds().contains(sf::Mouse::getPosition(*main_window).x, sf::Mouse::getPosition(*main_window).y)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				ballSpeed = 3.5f;
				speed = 3.5f;
				hostGameLoop();
				return;
			}
		}
		if (join_button.getGlobalBounds().contains(sf::Mouse::getPosition(*main_window).x, sf::Mouse::getPosition(*main_window).y)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				textInputWindow();
				joinGameLoop();
				return;
			}
		}
		if (local_button.getGlobalBounds().contains(sf::Mouse::getPosition(*main_window).x, sf::Mouse::getPosition(*main_window).y)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				gameLoop();
				return;
			}
		}

		sf::Event event;
		while (main_window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				main_window->close();
				break;
			}

		}
	}

}

void MainGame::gameLoop() {

	while (main_window->isOpen())
	{

		sf::Event event;
		while (main_window->pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				main_window->close();
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::R) {
					ball.setPosition(WIDTH / 2, HEIGHT / 2);
					ballXDirection *= -1;
				}
				if (event.key.code == sf::Keyboard::W) p1up = true;
				if (event.key.code == sf::Keyboard::S) p1down = true;
				if (event.key.code == sf::Keyboard::Up) p2up = true;
				if (event.key.code == sf::Keyboard::Down) p2down = true;
				break;

			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::W) p1up = false;
				if (event.key.code == sf::Keyboard::S) p1down = false;
				if (event.key.code == sf::Keyboard::Up) p2up = false;
				if (event.key.code == sf::Keyboard::Down) p2down = false;
				break;
			}

		}

		//p1 edge detection
		if (checkIntersection(paddle, uWall)) {
			p1up = false;
		}
		if (checkIntersection(paddle, dWall)) {
			p1down = false;
		}

		//p2 edge detection
		if (checkIntersection(paddle2, uWall)) {
			p2up = false;
		}
		if (checkIntersection(paddle2, dWall)) {
			p2down = false;
		}

		//ball edge detection
		if (checkIntersection(ball, uWall)) {
			ballYDirection = ballSpeed;
		}
		if (checkIntersection(ball, dWall)) {
			ballYDirection = -ballSpeed;
		}

		//ball paddle detection
		if (checkIntersection(ball, paddle)) {
			ballXDirection = ballSpeed;
		}
		if (checkIntersection(ball, paddle2)) {
			ballXDirection = -ballSpeed;
		}

		//movement vector for player 1
		p1movement = sf::Vector2f(0, 0);
		p2movement = sf::Vector2f(0, 0);
		ballMovement = sf::Vector2f(ballXDirection, ballYDirection);

		if (p1up)
			p1movement.y -= speed;
		if (p1down)
			p1movement.y += speed;

		if (p2up)
			p2movement.y -= speed;
		if (p2down)
			p2movement.y += speed;

		paddle.move(p1movement);
		paddle2.move(p2movement);
		ball.move(ballMovement);
		update();
	}

}
void MainGame::hostGameLoop() {
	sf::TcpListener listener;

	if (listener.listen(5403) != sf::Socket::Done) {
		std::cout << "No listen Error" << std::endl;
	}


	sf::Text ip;
	ip.setFont(myfont);
	ip.setCharacterSize(25);
	ip.setFillColor(sf::Color::Green);
	ip.setPosition(sf::Vector2f(10.0f, 10.0f));

	
	sf::TcpSocket client;

	ip.setString(sf::IpAddress::getLocalAddress().toString());

	main_window->clear();
	main_window->draw(ip);
	main_window->display();

	if (listener.accept(client) != sf::Socket::Done) {
		std::cout << "client not accepted" << std::endl;
	}

	//Send Starting Data to Player 2
	sendData(client, getHostData());

	while (main_window->isOpen())
	{
		//Receive player 2 starting position. 
		float * player2_state = receiveData(client);

		if (player2_state != NULL) { // Only update if we receive player 2 data; if not then keep old data; 
			paddle2.setPosition(sf::Vector2f(paddle2.getPosition().x, player2_state[0]));
			delete player2_state;
		}


		sf::Event event;
		while (main_window->pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				main_window->close();
				client.disconnect();
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::R) {
					ball.setPosition(WIDTH / 2, HEIGHT / 2);
					ballXDirection *= -1;
				}
				if (event.key.code == sf::Keyboard::W) p1up = true;
				if (event.key.code == sf::Keyboard::S) p1down = true;
				break;

			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::W) p1up = false;
				if (event.key.code == sf::Keyboard::S) p1down = false;
				break;
			}

		}

		//p1 edge detection
		if (checkIntersection(paddle, uWall)) {
			p1up = false;
		}
		if (checkIntersection(paddle, dWall)) {
			p1down = false;
		}




		//ball edge detection
		if (checkIntersection(ball, uWall)) {
			ballYDirection = ballSpeed;
		}
		if (checkIntersection(ball, dWall)) {
			ballYDirection = -ballSpeed;
		}

		//ball paddle detection
		if (checkIntersection(ball, paddle)) {
			ballXDirection = ballSpeed;
		}
		if (checkIntersection(ball, paddle2)) {
			ballXDirection = -ballSpeed;
		}

		//movement vector for player 1
		p1movement = sf::Vector2f(0, 0);

		ballMovement = sf::Vector2f(ballXDirection, ballYDirection);

		//player1 padde movement updater;
		if (p1up)
			p1movement.y -= speed;
		if (p1down)
			p1movement.y += speed;


		paddle.move(p1movement);
		ball.move(ballMovement);
		sendData(client, getHostData()); //send player1's current state to player2 
		update();
	}
	client.disconnect();

}
void MainGame::joinGameLoop() {  // Player2's Game Loop 
	sf::TcpSocket host;
	std::cout << ip_addr << std::endl;
	sf::Socket::Status status = host.connect(ip_addr, 5403);

	if (status != sf::Socket::Done) {
		std::cout << "No Connection" << std::endl;
		showErrorMessage();
		return;
	}


	while (main_window->isOpen()) {
		//Receive data from player1 
		float* player1_state = receiveData(host);

		if (player1_state != NULL) { // only update when data is received
			paddle.setPosition(sf::Vector2f(paddle.getPosition().x , player1_state[0]));
			ball.setPosition(sf::Vector2f(player1_state[1], player1_state[2]));
			delete player1_state;
		}

		sf::Event event;
		while (main_window->pollEvent(event))
		{

			switch (event.type) {
			case sf::Event::Closed:
				main_window->close();
				host.disconnect();
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::W) p2up = true;
				if (event.key.code == sf::Keyboard::S) p2down = true;
				break;

			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::W) p2up = false;
				if (event.key.code == sf::Keyboard::S) p2down = false;
				break;
			}


		}

		//Player2 edge detection
		if (checkIntersection(paddle2, uWall)) {
			p2up = false;
		}
		if (checkIntersection(paddle2, dWall)) {
			p2down = false;
		}

		p2movement = sf::Vector2f(0, 0);

		//player2 movement updater
		if (p2up)
			p2movement.y -= speed;
		if (p2down)
			p2movement.y += speed;


		paddle2.move(p2movement);
		sendData(host, getJoinData()); // send player 2 data to player 1
		update();

	}
	host.disconnect();

}
void MainGame::textInputWindow() {
	sf::Text ip_text;
	ip_text.setString("Enter Ip Address");

	sf::Text ip_addr_text;
	ip_addr_text.setString("");

	ip_text.setFont(myfont);
	ip_text.setCharacterSize(25);
	ip_text.setFillColor(sf::Color::Green);
	ip_text.setPosition(sf::Vector2f(10.0f, 10.0f));

	ip_addr_text.setFont(myfont);
	ip_addr_text.setCharacterSize(25);
	ip_addr_text.setFillColor(sf::Color::Green);
	ip_addr_text.setPosition(sf::Vector2f(main_window->getSize().x / 2, main_window->getSize().y / 2));

	main_window->clear();
	main_window->draw(ip_text);
	main_window->draw(ip_addr_text);
	main_window->display();

	while (main_window->isOpen()) {
		sf::Event event;
		while (main_window->pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				main_window->close();
				break;

			case sf::Event::TextEntered:
				if (event.text.unicode == '\b' && ip_addr.size() > 0) {
					ip_addr.pop_back();
				}
				else if (event.text.unicode == 13) {
					if (ip_addr.size() == 0) {
						break;
					}
					return;
				}
				else if (event.text.unicode < 128 && event.text.unicode != '\b') {
					ip_addr.push_back((char)event.text.unicode);
				}
				break;
			}

		}
		ip_addr_text.setString(ip_addr);
		main_window->clear();
		main_window->draw(ip_text);
		main_window->draw(ip_addr_text);
		main_window->display();
	}
	
}
void MainGame::update() {
	main_window->clear();
	main_window->draw(ball);
	main_window->draw(paddle);
	main_window->draw(paddle2);
	main_window->display();
}
void MainGame::showErrorMessage() {
	sf::Text Error;
	Error.setString("There Was An Error. Close The window and Start Again");
	Error.setFont(myfont);
	Error.setCharacterSize(25);
	Error.setFillColor(sf::Color::Red);
	Error.setPosition(sf::Vector2f(10.0f, 10.0f));

	main_window->clear();
	main_window->draw(Error);
	main_window->display();

	while (main_window->isOpen()) {
		sf::Event event;
		while (main_window->pollEvent(event))
		{
			switch (event.type) {
			case sf::Event::Closed:
				main_window->close();
				break;
			}
		}
	}
	main_window->clear();
	main_window->draw(Error);
	main_window->display();
}

std::string MainGame::getHostData() { // Utility function to convert host data into string
	std::string host_position = std::to_string(paddle.getPosition().y);
	std::string ball_position = std::to_string(ball.getPosition().x) + "," + std::to_string(ball.getPosition().y);
	host_position += ("," + ball_position);
	return host_position.c_str();
}

std::string MainGame::getJoinData() { // Utility function to convert player2 data into string
	std::string join_position = std::to_string(paddle2.getPosition().y)+",";
	return join_position;
}

float * MainGame::parseData(std::string data) { //Parser for incoming data; 
	float * game_data = new float[6];
	char basic_data[100];

	strcpy(basic_data, data.c_str());
	char * buff = strtok(basic_data, ",");

	for (int i = 0; buff != NULL; i++) {
		std::size_t size;
		game_data[i] = std::strtof(buff, NULL);
		buff = strtok(NULL, ",");
	}

	return game_data;

}

float * MainGame::receiveData(sf::TcpSocket& soc) { // Utility Function to Receive data
	char data[50];
	std::size_t receive;
	sf::Socket::Status stats = soc.receive(data, 50, receive);
	if (stats != sf::Socket::Done) {
		std::cout << "Error Receiving " << std::endl;
		soc.disconnect();
		return NULL;
	}
	return parseData(data);
	

}

void MainGame::sendData(sf::TcpSocket & soc, const std::string & data) { // Utility Function to Send Data; 

	char sdata[50];
	strcpy(sdata, data.c_str());

	if (soc.send(sdata, 50) != sf::Socket::Done) {
		std::cout << "Error Sending Data" << std::endl;
		soc.disconnect();
	}

}

sf::RectangleShape MainGame::createRectangle(int posX, int posY, int sizeX, int sizeY) {
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(sizeX, sizeY));
	rectangle.setPosition(posX, posY);
	rectangle.setFillColor(sf::Color::White);
	rectangle.setOutlineThickness(5);
	return rectangle;
}

bool MainGame::checkIntersection(sf::Shape& one, sf::Shape& two) {
	if (one.getGlobalBounds().intersects(two.getGlobalBounds())) {
		return true;
	}
	else return false;
}
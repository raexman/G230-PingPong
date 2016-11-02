// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"

void PlayMusic();
void LoadBackground();
void Init();
void Menu();
void PlayerVsCPUMatch();
void PlayerVsPlayerMatch();
void FourWayMatch();
void Victory();
void Exit();

float deltaTime;
bool isPlaying;
bool isMenu;
int matchMode;
float maxScore = 4;
const int numOfPlayers = 2;

sf::Font font;
sf::RenderWindow *window;
sf::Sprite background;
sf::Texture bgTexture;

int WinMain()
{
	font.loadFromFile("AmericanCaptain.ttf");

	window = new sf::RenderWindow(sf::VideoMode(800, 600), "PingPong!");
	bgTexture.loadFromFile("background.jpg");
	background.setTexture(bgTexture);

	Init();

	Exit();
	system("pause");
	return 0;

}
void Init()
{

	Menu();

}

void Menu()
{
	bool optionChosen = false;
	int mode = 0;

	while (window->isOpen() && !optionChosen)
	{
		sf::Text title;
		title.setFont(font);
		title.setString("ULTRA PONG 20XX");
		title.setCharacterSize(125);
		title.setFillColor(sf::Color::White);
		title.setPosition((window->getSize().x - title.getLocalBounds().width) * 0.5, 100);

		sf::Text menu;
		menu.setFont(font);
		menu.setString("[1] Player vs CPU \n[2] Player vs Player\n[3] 4-Player Game");
		menu.setCharacterSize(48);
		menu.setFillColor(sf::Color::White);
		menu.setPosition((window->getSize().x - menu.getLocalBounds().width) * 0.5, title.getPosition().y + title.getGlobalBounds().height + 50);

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
				return;
			}
		}

		window->clear();
		window->draw(background);
		window->draw(menu);
		window->draw(title);
		window->display();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
		{
			mode = 1;
			optionChosen = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
		{
			mode = 2;
			optionChosen = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
		{
			mode = 3;
			optionChosen = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
		{
			mode = 4;
			optionChosen = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			mode = 0;
			optionChosen = true;
		}
	}
	switch (mode)
	{
		case 0: Exit();
		case 1: PlayerVsCPUMatch(); break;
		case 2: PlayerVsPlayerMatch(); break;
		case 3: FourWayMatch(); break;
		case 4: PlayerVsCPUMatch(); break;
		default:break;
	}
}

void PlayerVsCPUMatch()
{
	Paddle player1(window, 0);
	Paddle player2(window, 1, true);

	Paddle *players[numOfPlayers] = { &player1, &player2};

	Ball ball(&players[0], numOfPlayers, window);

	sf::Clock clock;
	isPlaying = true;
	while (window->isOpen() && isPlaying)
	{
		window->setActive(true);
		deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
				return;
			}
		}

		window->clear();
		window->draw(background);
		player1.Update(deltaTime);
		player2.Update(deltaTime);
		ball.Update(deltaTime);
		window->display();

		for (int i = 0; i < numOfPlayers; i++)
		{
			if (players[i]->getScore() > maxScore)
			{
				isPlaying = false;
				break;
			}
		}
	}

	Victory();

}

void PlayerVsPlayerMatch()
{
	Paddle player1(window, 0);
	Paddle player2(window, 1);

	Paddle *players[numOfPlayers] = { &player1, &player2 };

	Ball ball(&players[0], numOfPlayers, window);

	sf::Clock clock;
	isPlaying = true;
	while (window->isOpen() && isPlaying)
	{
		window->setActive(true);
		deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
				return;
			}
		}

		window->clear();
		window->draw(background);
		player1.Update(deltaTime);
		player2.Update(deltaTime);
		ball.Update(deltaTime);
		window->display();

		for (int i = 0; i < numOfPlayers; i++)
		{
			if (players[i]->getScore() > maxScore)
			{
				isPlaying = false;
				break;
			}
		}
	}
	Victory();
}

void FourWayMatch()
{
	Paddle player1(window, 0);
	Paddle player2(window, 1, true);
	Paddle player3(window, 0, true, false, 400, 60, 20);
	Paddle player4(window, 1, true, false, 400, 60, 20);

	Paddle *players[4] = { &player1, &player2, &player3, &player4 };

	Ball ball(&players[0], 4, window);
	Ball ball2(&players[0], 4, window);

	sf::Clock clock;
	isPlaying = true;
	while (window->isOpen() && isPlaying)
	{
		window->setActive(true);
		deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
				return;
			}
		}

		window->clear();
		window->draw(background);
		player1.Update(deltaTime);
		player2.Update(deltaTime);
		player3.Update(deltaTime);
		player4.Update(deltaTime);
		ball.Update(deltaTime);
		ball2.Update(deltaTime);
		window->display();

		for (int i = 0; i < 4; i++)
		{
			if (players[i]->getScore() > maxScore)
			{
				isPlaying = false;
				break;
			}
		}
	}
	Victory();
}

void Victory()
{
	bool optionChosen = false;
	while (window->isOpen() && !optionChosen)
	{
		sf::Text menu;
		menu.setFont(font);
		menu.setString("GAME OVER!");
		menu.setCharacterSize(96);
		menu.setFillColor(sf::Color::White);
		menu.setPosition((window->getSize().x - menu.getLocalBounds().width) * 0.5, (window->getSize().y - menu.getLocalBounds().height) * 0.5);

		sf::Text subtitle;
		subtitle.setFont(font);
		subtitle.setString("Press [SPACEBAR] to go to Menu");
		subtitle.setCharacterSize(48);
		subtitle.setFillColor(sf::Color::White);
		subtitle.setPosition((window->getSize().x - subtitle.getLocalBounds().width) * 0.5, menu.getPosition().y + menu.getLocalBounds().height + 50);

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
				return;
			}
		}

		window->clear();
		window->draw(background);
		window->draw(menu);
		window->draw(subtitle);
		window->display();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			optionChosen = true;
		}
	}

	Menu();
}

void PlayMusic()
{

}

void LoadBackground()
{

}

void Exit()
{
}
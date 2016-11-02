#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
class Paddle
{
private:
	sf::RenderWindow *window;
	float width;
	float height;
	float margin = 0.1f;
	float speed;
	float ceiling = 0;
	float leftWall = 0;
	float rightWall;
	float floor;
	bool isAI;
	float followDirection;
	std::string label = "Score:";

	sf::Keyboard::Key upKey;
	sf::Keyboard::Key downKey;
	sf::Keyboard::Key leftKey;
	sf::Keyboard::Key rightKey;

public:
	int id;
	int score;
	bool isVertical;
	float deltaTime;
	sf::Text text;
	sf::Font font;
	sf::RectangleShape body;
	sf::SoundBuffer buffer;
	sf::Sound sound;
	Paddle(sf::RenderWindow * window,  int id, bool isAI = false, bool isVertical = true, float speed = 400, float width = 20, float height = 60);
	~Paddle();
	int getScore();
	void addScore();
	void Update(float deltaTime);
	void Draw();
	void Control();
	void NPC();
	void FollowBall(float x, float y);
	void Move(float x, float y);
};


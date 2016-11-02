#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include "Paddle.h"

class Ball
{
private:
	sf::RenderWindow *window;
	float radius;
	float margin = 0.1f;
	float ceiling = 0;
	float leftWall = 0;
	float rightWall;
	float floor;
	int reboundAngle = 20;
	int numOfPlayers;
	//Functions.
	void CheckCollision(Paddle *player);
public:
	Paddle *p1;
	Paddle *p2;
	Paddle **players;
	sf::CircleShape body;
	sf::SoundBuffer buffer;
	sf::Sound sound;
	float speed;
	float angle;
	float deltaTime;
	const static float PI;
	const static float DEGREES;
	//Functions.
	Ball(Paddle *p1, Paddle *p2, sf::RenderWindow *window, float speed = 400, float radius = 5);
	Ball(Paddle **players, int numOfPlayers, sf::RenderWindow *window, float speed = 400, float radius = 5);
	~Ball();
	void Move();
	void Update(float deltaTime);
	void Draw();
	void Bounce();
	void Collide();
	void Init();
	void AddSpeed(int rate = 1);
	static float degToRadians(float deg);
	static float getRandomAngle();
};


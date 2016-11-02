#include "Ball.h"

//Floats can't be initialized in the header.
const float Ball::PI = 3.14159265359f;
const float Ball::DEGREES = 360.0f;

Ball::Ball(Paddle *p1, Paddle *p2, sf::RenderWindow *window, float speed, float radius)
{
	this->speed = speed;
	this->radius = radius;

	//Player stuff.
	this->p1 = p1;
	this->p2 = p2;

	//Camera stuff.
	this->window = window;
	rightWall = window->getSize().x;
	floor = window->getSize().y;

	//Ball stuff.
	body = sf::CircleShape(radius);
	Init();

	//Clean up.
}

Ball::Ball(Paddle **players, int numOfPlayers, sf::RenderWindow *window, float speed, float radius)
{
	this->speed = speed;
	this->radius = radius;
	this->numOfPlayers = numOfPlayers;

	//Player stuff.
	this->players = players;
	//players = NULL;
	//delete players;

	//Camera stuff.
	this->window = window;
	rightWall = window->getSize().x;
	floor = window->getSize().y;

	//Ball stuff.
	body = sf::CircleShape(radius);
	Init();
}


Ball::~Ball()
{

}

void Ball::Update(float deltaTime) {
	
	this->deltaTime = deltaTime;

	Move();
	Bounce();
	Collide();
	Draw();
}
void Ball::Move()
{
	float velocity = speed * deltaTime;
	body.move(std::cos(angle) * velocity, std::sin(angle) * velocity);
	for (int i = 0; i < numOfPlayers; i++)
	{
		players[i]->FollowBall(body.getPosition().x, body.getPosition().y);
	}
}
void Ball::Bounce() {
	//Player 1 scores.
	if (body.getPosition().x + radius > rightWall)
	{
		players[0]->addScore();
		Init();
	}
	//Player 2 scores.
	else if (body.getPosition().x - radius < 0)
	{
		players[1]->addScore();
		Init();
	}
	else if (body.getPosition().y + radius > floor)
	{
		//Invert angle.
		if (numOfPlayers == 4)
		{
			players[2]->addScore();
			Init();
		}
		else
		{
			angle = -angle;
			body.setPosition(body.getPosition().x, floor - radius - 0.2f);
			buffer.loadFromFile("bumper.wav");
			sound.setBuffer(buffer);
			sound.play();
		}
	}
	else if (body.getPosition().y - radius < 0)
	{
		if (numOfPlayers == 4)
		{
			players[3]->addScore();
			Init();
		}
		else
		{
			//Invert angle.
			angle = -angle;
			body.setPosition(body.getPosition().x, radius + 0.2f);
			buffer.loadFromFile("bumper.wav");
			sound.setBuffer(buffer);
			sound.play();
		}
	}


}

//void Ball::Collide() {
//
//	//Ball vars.
//	float ballX = body.getPosition().x;
//	float ballY = body.getPosition().y;
//
//	//Paddle 1 vars.
//	float paddle1X = p1->body.getPosition().x;
//	float paddle1Y = p1->body.getPosition().y;
//	float paddle1Width = p1->body.getSize().x;
//	float paddle1Height = p1->body.getSize().y;
//
//	//Paddle 2 vars.
//	float paddle2X = p2->body.getPosition().x;
//	float paddle2Y = p2->body.getPosition().y;
//	float paddle2Width = p2->body.getSize().x;
//	float paddle2Height = p2->body.getSize().y;
//
//	//Check if ball is lesser than the left paddles right side, but greater than its left side.
//	float distX = std::cos(angle) * -1;
//	float distY = std::sin(angle);
//
//
//	if (
//		ballX < paddle1X + paddle1Width &&
//		ballX > paddle1X &&
//		ballY <= paddle1Y + paddle1Height &&
//		ballY + radius * 2 >= paddle1Y)
//	{
//
//		std::cout << "ANGLE :" << angle << std::endl;
//		//angle = std::atan2(distY, distX);
//
//		if(ballY + radius > paddle1Y + paddle1Height * 0.5)
//			angle = Ball::degToRadians(0 - angle + std::rand() % reboundAngle);
//		else
//			angle = Ball::degToRadians(0 - angle - std::rand() % reboundAngle);
//
//		body.setPosition(paddle1Width + radius * 2 + margin, ballY);
//		std::cout << "NU ANGLE :" << angle << std::endl;
//
//		AddSpeed();
//
//	}
//
//	if (
//		ballX + radius * 2 > paddle2X &&
//		ballX + radius * 2 < paddle2X + paddle2Width &&
//		ballY <= paddle2Y + paddle2Height &&
//		ballY + radius * 2 >= paddle2Y)
//	{
//		//angle = std::atan2(distY, distX);
//		//speed = 0;
//
//		std::cout << "ANGLE :" << angle << std::endl;
//		if (ballY + radius > paddle2Y + paddle2Height * 0.5)
//			angle = Ball::degToRadians(180 - angle - std::rand() % reboundAngle);
//		else
//			angle = Ball::degToRadians(180 - angle + std::rand() % reboundAngle);
//
//		body.setPosition(paddle2X - radius * 2 - margin, ballY);
//		std::cout << "NU ANGLE :" << angle << std::endl;
//
//		AddSpeed();
//	}
//}

void Ball::Collide() {

	for (int i = 0; i < numOfPlayers; i++)
	{
		CheckCollision(players[i]);
		//std::cout << "PADDLE RANGE X: " << (players+i)->body.getPosition().y << std::endl;
	}
}

void Ball::CheckCollision(Paddle *player)
{
	//Ball vars.
	sf::Vector2f ballPos(body.getPosition());
	sf::Vector2f ballDim(radius * 2, radius * 2);

	//Paddle 1 vars.
	sf::Vector2f paddlePos(player->body.getPosition());
	sf::Vector2f paddleDim(player->body.getSize());

	//Paddle vector.
	float paddleUnit = 0;
	sf::Vector2f paddleRange;
	float ballUnit;
	float ballCenter;
	sf::Vector2f ballRange;

	float horizon;

	switch (player->isVertical)
	{
			//TOP/BOTTOM
		case 0:
			//std::cout << "NON-VERTICAL!" << std::endl;
			if (player->id == 0)
			{
				//std::cout << "Player 1!" << std::endl;
				paddleUnit = paddlePos.y + paddleDim.y;
				ballUnit = ballPos.y;
				horizon = 90;
			}
			else
			{
				//std::cout << "Player 2!" << std::endl;
				paddleUnit = paddlePos.y;
				ballUnit = ballPos.y + ballDim.y;
				horizon = 270;
			}
			ballRange = sf::Vector2f(ballPos.x, ballPos.x + ballDim.x);
			paddleRange = sf::Vector2f(paddlePos.x, paddlePos.x + paddleDim.x);
			ballCenter = ballPos.x + radius;
			break;
			//LEFT/RIGHT
		case 1:
			//std::cout << "VERTICAL!" << std::endl;
			if (player->id == 0)
			{
				//std::cout << "Player 1!" << std::endl;
				paddleUnit = paddlePos.x + paddleDim.x;
				ballUnit = ballPos.x;
				//if (paddleUnit + radius > ballUnit)
				//{
				//	std::cout << "PADDLE UNIT: " << paddleUnit << std::endl;
				//	std::cout << "BALL UNIT: " << ballUnit << std::endl;
				//}
				horizon = 0;
			}
			else
			{
				//std::cout << "Player 2!" << std::endl;
				paddleUnit = paddlePos.x;
				ballUnit = ballPos.x + ballDim.x;
				horizon = 180;
			}
			ballRange = sf::Vector2f(ballPos.y, ballPos.y + ballDim.y);
			paddleRange = sf::Vector2f(paddlePos.y, paddlePos.y + paddleDim.y);
			ballCenter = ballPos.y + radius;
			break;
		default:
			break;
	}

	//Check if the absolute distance between units is smaller than or equal to 0.
	float distance = abs(paddleUnit - ballUnit);
	/*
	if (distance <= radius)
	{ 
		std::cout << "DISTANCE: " << distance << std::endl;
		std::cout << "BALL CENTER: " << ballCenter << std::endl;
		std::cout << "PADDLE RANGE X: " << paddleRange.x << std::endl;
		std::cout << "PADDLE RANGE Y: " << paddleRange.y << std::endl;

		//if(player->id == 1 && player->isVertical == false)
		//system("pause");
	}*/
	if (distance <= radius && ballRange.y > paddleRange.x && ballRange.x < paddleRange.y)
	{
		float distX = std::cos(angle);
		float distY = std::sin(angle);
		float angleRate;
		float newAngle;

		if (player->isVertical)
		{
			distX = -distX;
			angleRate = (ballPos.y + radius) - (paddlePos.y + paddleDim.y * 0.5);
			angleRate = angleRate / paddleDim.y;
			newAngle = 45;

			if (player->id == 0)
			{
				newAngle *= angleRate;
			}
			else
			{
				newAngle *= -angleRate;
			}

			//std::cout << "V: " << newAngle << " : " << angleRate << std::endl;
		}
		else
		{
			distY = -distY;
			angleRate = (ballPos.x + radius) - (paddlePos.x + paddleDim.x * 0.5);
			angleRate = angleRate / (paddleDim.x * 0.5);

			newAngle = 90;

			if (player->id == 0)
			{
				newAngle *= angleRate;
				newAngle += 45;
			}
			else
			{
				newAngle *= -1;
				newAngle *= angleRate;
				newAngle -= 45;
			}
			//std::cout << "H: " << newAngle << std::endl;
		}
		
		//distX = distX - std::cos(extraAngle);
		//distY = distY - std::sin(extraAngle);

		//std::cout << "ANGLE: " << angle * 180/Ball::PI << std::endl;
		angle = std::atan2(distY, distX);//+extraAngle;
		angle += degToRadians(newAngle) * 0.5;
		//std::cout << "NEW ANGLE: " << angle * 180 / Ball::PI << std::endl;
		//system("pause");
		/*
		std::cout << "COLLISION!" << std::endl;
		//Check if the ball unit is between the range.
		float paddleRangeCenter = (paddleRange.y - paddleRange.x) * 0.5f;
		float addedAngle = std::rand() % reboundAngle;
		float angleDirection = player->id == 0 ? 1 : -1;

		if (ballCenter > paddleRangeCenter)
			angle = Ball::degToRadians(horizon - angle + addedAngle * angleDirection);
		else
			angle = Ball::degToRadians(horizon - angle - addedAngle * angleDirection);

		//Move the ball to a safe zone.
		std::cout << "NU ANGLE :" << angle << std::endl;
		*/
		float rearrangePos = player->id == 0 ? 1 : -1;
		if (player->isVertical)
		{
			body.setPosition(ballUnit + (radius * 2 +margin)*rearrangePos, body.getPosition().y);
		}
		else
		{
			body.setPosition(body.getPosition().x, ballUnit + (radius * 2 + margin)*rearrangePos);
		}
		AddSpeed();
		buffer.loadFromFile("hit.wav");
		sound.setBuffer(buffer);
		sound.play();
	}
}

void Ball::Draw()
{
	window->draw(body);
}

float Ball::getRandomAngle()
{
	float randAngle = std::rand() % 90 - 45;
	float randDir = std::rand() % 2;

	return Ball::degToRadians(randAngle + (Ball::DEGREES * 0.5 * randDir));
}

float Ball::degToRadians(float deg)
{
	return (deg * 2 * Ball::PI) / Ball::DEGREES;
}

void Ball::Init()
{
	speed = 400;
	body.setFillColor(sf::Color::Red);
	body.setPosition(window->getSize().x * 0.5, window->getSize().y * 0.5);
	angle = Ball::getRandomAngle();
}

void Ball::AddSpeed(int rate)
{
	speed+=rate;
}

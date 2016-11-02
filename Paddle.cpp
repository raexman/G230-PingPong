#include "Paddle.h"


Paddle::Paddle(sf::RenderWindow * window, int id, bool isAI, bool isVertical, float speed, float width, float height)
{
	this->id = id;
	this->isAI = isAI;
	this->isVertical = isVertical;
	this->speed = speed;
	this->width = width;
	this->height = height;

	if (!isAI)
	{
		if (id == 0)
		{
			upKey = sf::Keyboard::Up;
			downKey = sf::Keyboard::Down;
			leftKey = sf::Keyboard::Left;
			rightKey = sf::Keyboard::Right;
		}
		else
		{
			upKey = sf::Keyboard::W;
			downKey = sf::Keyboard::S;
			leftKey = sf::Keyboard::A;
			rightKey = sf::Keyboard::D;
		}
	}

	//Camera stuff.
	this->window = window;
 	rightWall = window->getSize().x;
	floor = window->getSize().y;
	
	//Paddle stuff.
	body = sf::RectangleShape(sf::Vector2f(width, height));
	body.setFillColor(sf::Color::White);

	//Text
	if(!font.loadFromFile("AmericanCaptain.ttf"))
	{
		std::cout << " WTOIPSJFPOSDJFPOFJID " << std::endl;
	}

	text.setFont(font);
	std::string newLabel = label;
	text.setString(newLabel += std::to_string(score));
	text.setCharacterSize(24); // in pixels, not points!
	text.setFillColor(sf::Color::White);

	if (isVertical)
	{
		body.setPosition(id * (rightWall - width), (floor - height) * 0.5f);
		text.setPosition(id * (rightWall - text.getLocalBounds().width), 0);
	}
	else
	{
		body.setPosition((rightWall - width)*0.5f, id * (floor - height));
		text.setPosition(id * (rightWall - text.getLocalBounds().width - margin), (floor - text.getLocalBounds().height - margin * 2));
	}
}

Paddle::~Paddle()
{
}

void Paddle::Update(float deltaTime)
{
	this->deltaTime = deltaTime;
	if (isAI)
	{
		NPC();
	}
	else
	{
		Control();
	}
	Draw();
}

void Paddle::addScore()
{
	score++;
	std::string newLabel = label;
	newLabel += std::to_string(score);
	text.setString(newLabel);
	buffer.loadFromFile("cheers.ogg");
	sound.setBuffer(buffer);
	sound.play();

}

int Paddle::getScore()
{
	return this->score;
}

void Paddle::Move(float moveX, float moveY)
{
	float bodyX = body.getPosition().x;
	float bodyY = body.getPosition().y;
	float bodyWidth = body.getSize().x;
	float bodyHeight = body.getSize().y;

	float velocity = speed * deltaTime;

	body.move(moveX * deltaTime, moveY * deltaTime);

	if (bodyX < 0)
	{
		body.setPosition(margin, bodyY);
	}
	else if (bodyX + bodyWidth > rightWall)
	{
		body.setPosition(rightWall - bodyWidth - margin, bodyY);
	}

	if (bodyY < 0)
	{
		body.setPosition(bodyX, margin);
	}
	else if (bodyY + bodyHeight > floor)
	{
		body.setPosition(bodyX, floor - bodyHeight - margin);
	}
}

void Paddle::Control()
{
	if (!isVertical)
	{
		//MOVE LEFT.
		if (sf::Keyboard::isKeyPressed(upKey))
		{
			Move(-speed, 0);
			body.setFillColor(sf::Color::Red);
		}

		//MOVE RIGHT.
		if (sf::Keyboard::isKeyPressed(rightKey))
		{
			Move(speed, 0);
			body.setFillColor(sf::Color::Blue);
		}
	}

	if (isVertical)
	{
		//MOVE UP.
		if (sf::Keyboard::isKeyPressed(upKey))
		{
			Move(0, -speed);
			body.setFillColor(sf::Color::Red);
		}

		//MOVE DOWN.
		if (sf::Keyboard::isKeyPressed(downKey))
		{
			Move(0, speed);
			body.setFillColor(sf::Color::Blue);
		}
	}
}

void Paddle::NPC()
{
}

void Paddle::FollowBall(float x, float y)
{
	if (isAI)
	{
		if (isVertical)
		{
			float distance = y - body.getPosition().y;
			int direction = distance > 0 ? 1 : (distance < 0 ? -1 : 0);
			Move(0, direction * (std::rand() % (int)speed));
		}
		else
		{
			float distance = x - body.getPosition().x;
			int direction = distance > 0 ? 1 : (distance < 0 ? -1 : 0);
			Move(direction * (std::rand() % (int)speed), 0);
		}
	}
}

void Paddle::Draw()
{
	sf::RectangleShape anchor(sf::Vector2f(10, 10));
	anchor.setFillColor(sf::Color::Magenta);
	anchor.setPosition(body.getPosition().x, body.getPosition().y);

	window->draw(body);
	//window->draw(anchor);
	window->draw(text);
}
#include "Portal.h"

Portal::Portal()
{
}

Portal::Portal(int type, float x, float y, float velocity, int invertedX, int invertedY)
{
	sprite.setPosition(x, y);
	sprite.setOrigin(16, 24);
	sprite.setScale(3 * invertedX, 3 * invertedY);

	this->type = type;
	this->velocity = sf::Vector2f(velocity, 0);
	velocityFactor = invertedX;
	this->inverted = invertedX;
}

Portal::~Portal()
{
}

sf::Vector2i Portal::Update(float deltaTime)
{
	Move(deltaTime);

	animTime += deltaTime;
	sf::Vector2i res(-1, type);

	if (animTime > 0.05f)
	{
		animTime -= 0.05f;
		index++;
		res.x = index % indexMax;
	}

	return res;
}

void Portal::Mirror(float perc)
{
	if (!isMirroring)
	{
		isMirroring = true;
		xPosHome = sprite.getPosition().x;
		xPosDest = 1080 - xPosHome;
		velocityFactor *= -1;
	}


	sprite.setPosition(xPosDest + (1.0f - perc) * (xPosHome - xPosDest), sprite.getPosition().y);
	sprite.setScale(3 * cosf(3.1415f * perc) * inverted, sprite.getScale().y);

	if (perc == 1.0f)
		isMirroring = false;
}

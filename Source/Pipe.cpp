#include "Pipe.h"

Pipe::Pipe()
{
}

Pipe::Pipe(sf::Texture& texture, float x, float y, float velocity, int inverted)
{
	SetTexture(texture);
	if (inverted == 1)
	{
		yPosition = -720 + y;
		yPosHome = 720;
	}
	else
	{
		yPosition = y;
		yPosHome = 0;
	}
	sprite.setPosition(x, yPosHome);
	sprite.setOrigin(texture.getSize().x / 2, 0);
	sprite.setScale(4, 4 * inverted);

	this->velocity = sf::Vector2f(velocity, 0);

	if (inverted == -1)
		rewardGiven = true;
}

Pipe::~Pipe()
{
}

void Pipe::Update(float deltaTime)
{
	Move(deltaTime);

	if (yMoveTime < 1)
	{
		yMoveTime += deltaTime;

		sprite.setPosition(sprite.getPosition().x, yPosHome + (yMoveTime / 1.0f) * yPosition);
	}
}

void Pipe::Mirror(float perc)
{
	if (!isMirroring)
	{
		isMirroring = true;
		xPosHome = sprite.getPosition().x;
		xPosDest = 1080 - xPosHome;
		velocityFactor *= -1;
	}


	sprite.setPosition(xPosDest + (1.0f - perc) * (xPosHome - xPosDest), sprite.getPosition().y);
	sprite.setScale(4 * cosf(3.1415f * perc), sprite.getScale().y);

	if (perc == 1.0f)
		isMirroring = false;

}

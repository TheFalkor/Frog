#include "Layer.h"

Layer::Layer()
{
}

Layer::Layer(sf::Texture& texture, float x, float y, float velocity)
{
	SetTexture(texture);
	sprite.setPosition(x, y);
	sprite.setOrigin(texture.getSize().x / 2, 0);

	this->velocity = sf::Vector2f(velocity, 0);
}

Layer::~Layer()
{
}

void Layer::Update(float deltaTime)
{
	Move(deltaTime);
}

void Layer::FadeColor(float perc)
{
	if (perc >= 1)
		perc = 1;

	float factor = perc;

	if (!darkmode)
		factor = 1 - factor;

	sprite.setColor(sf::Color(redShift + (255 - redShift) * factor, greenShift + (255 - greenShift) * factor, blueShift + (255 - blueShift) * factor, 255));


	if (perc == 1)
		darkmode = !darkmode;
}

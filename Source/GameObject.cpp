#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::SetPosition(float x, float y)
{
	sprite.setPosition(x, y);
}

void GameObject::SetTexture(sf::Texture& texture)
{
	sprite.setTexture(texture);
}

void GameObject::DrawSprite(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void GameObject::Move(float deltaTime)
{
	sprite.move(velocity * (deltaTime * velocityFactor));
}

bool GameObject::isColliding(sf::FloatRect hitbox)
{
	return sprite.getGlobalBounds().intersects(hitbox);
}



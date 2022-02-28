#pragma once

#include "SFML/Graphics.hpp"

class GameObject
{
public:
	GameObject();
	~GameObject();
	void SetPosition(float x, float y);
	void SetTexture(sf::Texture& texture);
	void DrawSprite(sf::RenderWindow& window);

	void Move(float deltaTime);
	bool isColliding(sf::FloatRect hitbox);


	sf::Sprite sprite;
	sf::Vector2f velocity;
	int velocityFactor = 1;

	bool doWorldWrap = false;
};
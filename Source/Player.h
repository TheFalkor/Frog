#pragma once

#include "GameObject.h"
#include "SFML/Audio.hpp"


class Player : public GameObject
{
public:
	Player();
	~Player();

	void Update(float deltaTime);
	void Jump();
	void Mirror(float perc);
	void Flip(float perc);

	sf::CircleShape hitbox;
	int scaleFactor = 1;

private:
	float jumpCooldown = -1.0f;
	float jumpCooldownDefault = 0.11f;

	float gravity = 0;
	int gravityFactor = 1;

	bool isMirroring = false;
	bool isFlipping = false;
	float rotMemory = 0;

	sf::Texture texture;
};
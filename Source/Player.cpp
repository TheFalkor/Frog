#include "Player.h"
#include <math.h>

Player::Player()
{
	if (!texture.loadFromFile("../Assets/Sprites/Frog.png"))
		system("pause");

	SetTexture(texture);
	SetPosition(540, 360);
	sprite.setOrigin(8, 8);
	sprite.setScale(4, 4);

	hitbox = sf::CircleShape(15);
	hitbox.setOrigin(15, 15);
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	jumpCooldown -= deltaTime;
	if (jumpCooldown > 0)
	{
		gravity -= 0.1f;
		gravity -= 0.3f;
	}


	gravity += 0.1f;

	if (gravity > 5)
		gravity = 5;

	sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + gravity * gravityFactor);


	float rotAngle = (gravity / 5) * 45;

	sprite.setRotation(scaleFactor * rotAngle * gravityFactor);

	hitbox.setPosition(sprite.getPosition());
}

void Player::Jump()
{
	if (jumpCooldown > -0.3f)
		return;
		
	gravity = 0;
	jumpCooldown = jumpCooldownDefault;
}

void Player::Mirror(float perc)
{
	if (!isMirroring)
	{
		isMirroring = true;
		scaleFactor *= -1;
		rotMemory = sprite.getRotation();

		if (rotMemory > 45)
			rotMemory = rotMemory - 360;
	}


	sprite.setScale(4 * cosf(3.1415f * perc) * scaleFactor * -1, 4 * gravityFactor);
	sprite.setRotation(rotMemory * cosf(3.1415f * perc));

	if (perc == 1.0f)
		isMirroring = false;
}

void Player::Flip(float perc)
{
	if (!isFlipping)
	{
		isFlipping = true;
		gravityFactor *= -1;
	}

	sprite.setScale(4 * scaleFactor, 4 * cosf(3.1415f * perc) * gravityFactor * -1);

	if (perc == 1.0f)
	{
		isFlipping = false;
		gravity = 0;
	}
}



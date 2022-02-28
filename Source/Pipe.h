#pragma once
#include "GameObject.h"

class Pipe : public GameObject
{
public:
	Pipe();
	Pipe(sf::Texture& texture, float x, float y, float velocity, int inverted);
	~Pipe();
	void Update(float deltaTime);
	void Mirror(float perc);
	bool rewardGiven = false;
private:
	bool isMirroring = false;
	float xPosHome = -1;
	float xPosDest = -1;

	int yPosition = -1;
	float yMoveTime = 0;
	int yPosHome = 0;

};
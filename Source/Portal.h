#pragma once
#include "GameObject.h"

class Portal : public GameObject
{
public:
	Portal();
	Portal(int type, float x, float y, float velocity, int invertedX, int invertedY);
	~Portal();
	sf::Vector2i Update(float deltaTime);
	void Mirror(float perc);
	bool consumed = false;
private:
	int type = -1;
	int index = 0;
	int indexMax = 20;
	float animTime = 0;

	int inverted = 1;
	bool isMirroring = false;
	float xPosHome = -1;
	float xPosDest = -1;
};
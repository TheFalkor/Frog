#pragma once
#include "GameObject.h"

class Layer : public GameObject
{
public:
	Layer();
	Layer(sf::Texture& texture, float x, float y, float velocity);
	~Layer();
	void Update(float deltaTime);
	void FadeColor(float perc);
private:
	float redShift = 220;
	float greenShift = 110;
	float blueShift = 230;
	bool darkmode = false;
};
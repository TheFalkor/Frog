#pragma once

#include "SFML/Graphics.hpp"
#include "Player.h"
#include "Pipe.h"
#include "Layer.h"
#include "Portal.h"
#include <vector>
#include "SFML/Audio.hpp"

enum class Screen
{
	MENU,
	GAME,
	END
};

class Game
{
public:
	Game(unsigned int &screenWidth, unsigned int &screenHeight, const char* &title);
	~Game();

	bool Update(float deltaTime); 
	void Draw(sf::RenderWindow& window);
private:
	void CheckInput();
	void SpawnPipe();
	void RestartGame();

	bool godmode = false;
	float keyCooldown = 0;

	int score = 0;
	int pipeCount = 0;
	Screen screen = Screen::MENU;
	Player player;
	std::vector<Pipe> pipeList;
	std::vector<Layer> backgroundList;
	std::vector<Layer> cloudList;
	std::vector<Portal> portalList;

	float spawnCooldown = 0;
	float restartCooldown = 0;

	sf::Texture pipeTexture;
	sf::Texture skyTexture;
	sf::Texture cloudTexture;

	std::vector<sf::Texture> reversedTextures;
	std::vector<sf::Texture> flipTextures;

	sf::Sprite flashSprite;
	sf::Texture flashTexture;

	float menuIdleValue = 0.5f;
	float flashValue = 0;
	bool doFlash = false;

	bool mirrorAnimating = false;
	float mirrorTime = 0;
	int velocityFactor = 1;

	bool flipAnimating = false;
	float flipTime = 0;
	int gravityMemory = 1;

	sf::Font font;
	sf::Text scoreText;
};
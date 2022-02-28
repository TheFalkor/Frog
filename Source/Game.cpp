// Henrik Nilsson 2021-01-15
#include "Game.h"
#include <iostream>
#include <fstream>

Game::Game(unsigned int &screenWidth, unsigned int &screenHeight, const char* &title)
{
	screenWidth = 1080;
	screenHeight = 720;
	title = "Geometry Bird";

	srand(time(0));

	if (!font.loadFromFile("../Assets/Font/arial.ttf"))
		system("pause");


	scoreText.setFont(font);
	scoreText.setCharacterSize(80);
	scoreText.setPosition(520, 60);
	scoreText.setString(std::to_string(score));


	for (int i = 0; i < 20; i++)
		reversedTextures.push_back(sf::Texture());
	for (int i = 0; i < 20; i++)
		if (!reversedTextures[i].loadFromFile("../Assets/Sprites/Reversed-" + std::to_string(i) + ".png"))
			system("pause");

	for (int i = 0; i < 20; i++)
		flipTextures.push_back(sf::Texture());
	for (int i = 0; i < 20; i++)
		if (!flipTextures[i].loadFromFile("../Assets/Sprites/Flipped-" + std::to_string(i) + ".png"))
			system("pause");


	if (!pipeTexture.loadFromFile("../Assets/Sprites/Pipe.png"))
		system("pause");

	if (!skyTexture.loadFromFile("../Assets/Sprites/Sky.png"))
		system("pause");

	if (!cloudTexture.loadFromFile("../Assets/Sprites/Cloud.png"))
		system("pause");

	if (!flashTexture.loadFromFile("../Assets/Sprites/Flash.png"))
		system("pause");

	flashSprite.setTexture(flashTexture);
	flashSprite.setScale(10, 10);
	flashSprite.setColor(sf::Color(255, 255, 255, 0));


	backgroundList.push_back(Layer(skyTexture, 200, 0, -100));
	backgroundList.push_back(Layer(skyTexture, 595, 0, -100));
	backgroundList.push_back(Layer(skyTexture, 990, 0, -100));
	backgroundList.push_back(Layer(skyTexture, 1385, 0, -100));

	cloudList.push_back(Layer(cloudTexture, 200, 0, -60));
	cloudList.push_back(Layer(cloudTexture, 595, 0, -60));
	cloudList.push_back(Layer(cloudTexture, 990, 0, -60));
	cloudList.push_back(Layer(cloudTexture, 1385, 0, -60));
};

Game::~Game()
{
}

bool Game::Update(float deltaTime)
{
	CheckInput();

	if (screen == Screen::MENU)
	{
		for (int i = 0; i < backgroundList.size(); i++)
		{
			backgroundList[i].Update(deltaTime);
			
			if (backgroundList[i].sprite.getPosition().x <= -200)
				backgroundList[i].sprite.setPosition(backgroundList[(i - 1) % (size_t)4].sprite.getPosition().x + 395, 0);
		}

		for (int i = 0; i < cloudList.size(); i++)
		{
			cloudList[i].Update(deltaTime);

			if (cloudList[i].sprite.getPosition().x <= -200)
				cloudList[i].sprite.setPosition(cloudList[(i - 1) % (size_t)4].sprite.getPosition().x + 395, 0);
		}

		menuIdleValue += deltaTime;
		player.sprite.setPosition(540, 360 + sinf(menuIdleValue * 5) * 30);

		return true;
	}
	if (screen == Screen::END)
	{
		if (doFlash)
		{
			flashValue += deltaTime;
			flashSprite.setColor(sf::Color(255, 255, 255, sinf(flashValue * 20) * 255));

			if (flashValue > 0.15f)
			{
				flashValue = 0;
				doFlash = false;
				flashSprite.setColor(sf::Color(255, 255, 255, 0));
			}
		}

		for (int i = 0; i < backgroundList.size(); i++)
		{
			cloudList[i].Update(deltaTime / 4);

			if (cloudList[i].sprite.getPosition().x <= -200)
				cloudList[i].sprite.setPosition(cloudList[(i - 1) % (size_t)4].sprite.getPosition().x + 395, 0);
		}

		restartCooldown -= deltaTime;
		player.Update(deltaTime);

		return true;
	}


	if (mirrorAnimating)
	{
		mirrorTime += deltaTime;
		float perc = mirrorTime / 0.5f;

		for (int i = 0; i < backgroundList.size(); i++)
			backgroundList[i].FadeColor(perc);


		if (perc >= 1)
		{
			perc = 1;
			mirrorTime = 0;
			mirrorAnimating = false;
			velocityFactor *= -1;
		}


		player.Mirror(perc);

		for (int i = 0; i < pipeList.size(); i++)
			pipeList[i].Mirror(perc);

		for (int i = 0; i < portalList.size(); i++)
			portalList[i].Mirror(perc);

		return true;
	}

	if (flipAnimating)
	{
		flipTime += deltaTime;
		float perc = flipTime / 0.5f;

		if (perc >= 1)
		{
			perc = 1;
			flipTime = 0;
			flipAnimating = false;
			gravityMemory *= -1;
		}

		player.Flip(perc);

		return true;
	}


	spawnCooldown -= deltaTime;
	if (spawnCooldown <= 0)
	{
		spawnCooldown = 1.5f + (rand() % 200) / 1000.0f;
		SpawnPipe();
	}

	for (int i = 0; i < backgroundList.size(); i++)
	{
		backgroundList[i].Update(deltaTime * velocityFactor);

		if (-540 * velocityFactor + backgroundList[i].sprite.getPosition().x * velocityFactor <= -740)
			backgroundList[i].sprite.setPosition(backgroundList[(i - 1 * velocityFactor) % (size_t)4].sprite.getPosition().x + 395 * velocityFactor, 0);
	}
	for (int i = 0; i < backgroundList.size(); i++)
	{
		cloudList[i].Update(deltaTime * velocityFactor);

		if (-540 * velocityFactor + cloudList[i].sprite.getPosition().x * velocityFactor <= -740)
			cloudList[i].sprite.setPosition(cloudList[(i - 1 * velocityFactor) % (size_t)4].sprite.getPosition().x + 395 * velocityFactor, 0);
	}

	for (int i = 0; i < pipeList.size(); i++)
	{
		pipeList[i].Update(deltaTime);
		bool giveReward = false;

		if (!pipeList[i].rewardGiven)
		{
			if (velocityFactor == 1 && pipeList[i].sprite.getPosition().x <= 540)
				giveReward = true;
			else if (velocityFactor == -1 && pipeList[i].sprite.getPosition().x >= 540)
				giveReward = true;
		}


		if (giveReward)
		{
			pipeList[i].rewardGiven = true;
			score++;
			scoreText.setString(std::to_string(score));
		}

		if (-540 * velocityFactor + pipeList[i].sprite.getPosition().x * velocityFactor <= -740)
		{
			pipeList.erase(pipeList.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < portalList.size(); i++)
	{
		sf::Vector2i result = portalList[i].Update(deltaTime);
		if (result.x != -1)
		{
			switch (result.y)
			{
				case 0:
					portalList[i].SetTexture(reversedTextures[result.x]);
					break;
				case 1:
					portalList[i].SetTexture(flipTextures[result.x]);
					break;
			}
		}


		bool consume = false;
		if (!portalList[i].consumed)
		{
			if (velocityFactor == 1 && portalList[i].sprite.getPosition().x <= 540)
				consume = true;
			else if (velocityFactor == -1 && portalList[i].sprite.getPosition().x >= 540)
				consume = true;
		}

		if (consume)
		{
			portalList[i].consumed = true;
			if (result.y == 0)
				mirrorAnimating = true;
			else
				flipAnimating = true;
		}

		if (-540 * velocityFactor + portalList[i].sprite.getPosition().x * velocityFactor <= -740)
		{
			portalList.erase(portalList.begin() + i);
			i--;
		}
	}

	player.Update(deltaTime);

	keyCooldown -= deltaTime;
	if (godmode)
		return true;

	for (int i = 0; i < pipeList.size(); i++)
	{
		if (pipeList[i].isColliding(player.hitbox.getGlobalBounds()))
		{
			restartCooldown = 1;
			screen = Screen::END;
			doFlash = true;
			break;
		}
	}

	if (player.sprite.getPosition().y < 0 || player.sprite.getPosition().y > 720)
	{
		restartCooldown = 1;
		screen = Screen::END;
		doFlash = true;
		return true;
	}

	return true;
}

void Game::SpawnPipe()
{
	pipeCount++;
	int distance = 175 + rand() % 75;
	int centerHeight = distance/2 + 75 + rand() % (720 - distance - 150);
	
	pipeList.push_back(Pipe(pipeTexture, 540 + 660 * velocityFactor, centerHeight - distance/2, -200 * velocityFactor, -1));
	pipeList.push_back(Pipe(pipeTexture, 540 + 660 * velocityFactor, centerHeight + distance/2, -200 * velocityFactor, 1));

	if (pipeCount % 3 == 0)
	{
		int type = rand() % 2;
		if(type == 0)
			portalList.push_back(Portal(type, 540 + 660 * velocityFactor, centerHeight, -200, velocityFactor, 1));
		else 
			portalList.push_back(Portal(type, 540 + 660 * velocityFactor, centerHeight, -200, velocityFactor, gravityMemory));
	}
}

void Game::RestartGame()
{
	player.sprite.setPosition(540, 360);
	player.sprite.setRotation(0);
	score = 0;
	scoreText.setString(std::to_string(score));
	pipeCount = 0;

	portalList.clear();
	pipeList.clear();
	menuIdleValue = 0;
	restartCooldown = 0;

	if (velocityFactor == -1)
	{
		player.Mirror(1);
		for (int i = 0; i < backgroundList.size(); i++)
			backgroundList[i].FadeColor(1);
	}

	if (gravityMemory == -1)
		player.Flip(1);

	mirrorAnimating = false;
	velocityFactor = 1;
	mirrorTime = 0;
	gravityMemory = 1;

	for(int i = 0; i < backgroundList.size(); i++)
		backgroundList[i].sprite.setColor(sf::Color(255, 255, 255, 255));
}

void Game::CheckInput()
{
	if (screen == Screen::GAME)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			player.Jump();

		if (keyCooldown <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			godmode = !godmode;
			keyCooldown = 0.5f;
		}
	}
	else if (screen == Screen::MENU)
	{
		if (menuIdleValue >= 0.5f && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			player.Jump();
			screen = Screen::GAME;
		}
	}
	else if (screen == Screen::END)
	{
		if (restartCooldown <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			screen = Screen::MENU;
			RestartGame();
		}
	}
}

void Game::Draw(sf::RenderWindow& window)
{	
	for (int i = 0; i < backgroundList.size(); i++)
		backgroundList[i].DrawSprite(window);

	for (int i = 0; i < cloudList.size(); i++)
		cloudList[i].DrawSprite(window);

	for (int i = 0; i < pipeList.size(); i++)
		pipeList[i].DrawSprite(window);

	for (int i = 0; i < portalList.size(); i++)
		portalList[i].DrawSprite(window);

	player.DrawSprite(window);
	window.draw(flashSprite);
	if (screen == Screen::MENU)
	{

	}
	else if (screen == Screen::GAME)
	{
		window.draw(scoreText);
	}
	else if (screen == Screen::END)
	{
		window.draw(scoreText);
	}
}
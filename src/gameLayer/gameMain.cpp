#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>

struct GameData
{
	float posX = 100;
	float posY = 100;
}gameData;

bool initGame()
{
	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	if (IsKeyDown(KEY_A)) { gameData.posX -= 200 * deltaTime; }
	if (IsKeyDown(KEY_D)) { gameData.posX += 200 * deltaTime; }
	if (IsKeyDown(KEY_W)) { gameData.posY -= 200 * deltaTime; }
	if (IsKeyDown(KEY_S)) { gameData.posY += 200 * deltaTime; }

	DrawRectangle(gameData.posX, gameData.posY, 50, 50, RED);

	return true;
}

void closeGame()
{
	// Resets the game state
	gameData = {};
}
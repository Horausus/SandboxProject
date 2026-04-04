#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>
#include <raymath.h>

struct GameData
{
	GameMap gameMap;
	Camera2D camera;
}gameData;

AssetManager assetManager;

bool initGame()
{
	assetManager.loadAll();

	gameData.gameMap.create(700, 500);

	for (int i = 0; i < 700; i++)
		for (int j = 0; j < 500; j++)
		{
			gameData.gameMap.getBlockUnsafe(i, j).type = Block::stone;
		}

	gameData.gameMap.getBlockUnsafe(0, 0).type = Block::dirt;
	gameData.gameMap.getBlockUnsafe(1, 1).type = Block::grassBlock;
	gameData.gameMap.getBlockUnsafe(2, 2).type = Block::goldBlock;
	gameData.gameMap.getBlockUnsafe(3, 3).type = Block::glass;
	gameData.gameMap.getBlockUnsafe(3, 3).type = Block::furnace;

	gameData.camera.target = { 0, 0 }; // world-space center of view, we will use this as the camera position
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 100.0f;

	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	gameData.camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	ClearBackground({ 75, 75, 150, 255 });

#pragma region camera movement
	if (IsKeyDown(KEY_LEFT)) gameData.camera.target.x -= 7.f * deltaTime;
	if (IsKeyDown(KEY_RIGHT)) gameData.camera.target.x += 7.f * deltaTime;
	if (IsKeyDown(KEY_UP)) gameData.camera.target.y -= 7.f * deltaTime;
	if (IsKeyDown(KEY_DOWN)) gameData.camera.target.y += 7.f * deltaTime;
#pragma endregion

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		auto block = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (block)
		{
			*block = {};
		}
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		auto block = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (block)
		{
			block->type = Block::woodLog;
		}
	}



#pragma region draw world
	BeginMode2D(gameData.camera);
	
	Vector2 topLeftView = GetScreenToWorld2D({ 0, 0 }, gameData.camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, gameData.camera);

	int startXView = (int)floorf(topLeftView.x - 1);
	int endXView = (int)ceilf(bottomRightView.x + 1);
	int startYView = (int)floorf(topLeftView.y - 1);
	int endYView = (int)ceilf(bottomRightView.y + 1);

	startXView = Clamp(startXView, 0, gameData.gameMap.w - 1);
	endXView = Clamp(endXView, 0, gameData.gameMap.w - 1);

	startYView = Clamp(startYView, 0, gameData.gameMap.h - 1);
	endYView = Clamp(endYView, 0, gameData.gameMap.h - 1);
	

	for (int y = startYView; y <= endYView; y++)
		for (int x = startXView; x <= endXView; x++)
		{

			auto& b = gameData.gameMap.getBlockUnsafe(x, y);

			if (b.type != Block::air)
			{
				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(b.type, 0, 32, 32), //source
					{ (float)x, (float)y, 1, 1}, //dest
					{ 0, 0 }, //origin (top-left corner)
					0.0f, //rotation
					WHITE //tint
				);
			}

			Rectangle src = getTextureAtlas(b.type, 0, 32, 32);

			if (b.type == Block::woodLog)
			{
				auto below = gameData.gameMap.getBlockSafe(x, y + 4);
				auto above = gameData.gameMap.getBlockSafe(x, y - 4);
				auto left = gameData.gameMap.getBlockSafe(x - 1, y);
				auto right = gameData.gameMap.getBlockSafe(x + 1, y);

				if (below && below->type == Block::woodLog)
				{
					src = getWoodLogTextureAtlas(/* top/middle variant here */ 5, 0, 32, 16);

					DrawTexturePro(
						assetManager.woodLog, // or woodLog if separate atlas
						src,
						{ (float)x, (float)y, 1, 1 },
						{ 0, 0 },
						0.0f,
						WHITE
					);
				}
				else if (above && above->type == Block::woodLog)
				{
					if (src == getWoodlogTextureAtlas())
					src = getWoodLogTextureAtlas(/* top/middle variant here */ 4, 0, 32, 16);

					DrawTexturePro(
						assetManager.woodLog, // or woodLog if separate atlas
						src,
						{ (float)x, (float)y, 1, 1 },
						{ 0, 0 },
						0.0f,
						WHITE
					);
				}
				else if (left && left->type == Block::woodLog)
				{
					src = getTextureAtlas(/* left variant here */ 12, 0, 32, 32);

					DrawTexturePro(
						assetManager.textures, // or woodLog if separate atlas
						src,
						{ (float)x, (float)y, 1, 1 },
						{ 0, 0 },
						0.0f,
						WHITE
					);
				}
				else if (right && right->type == Block::woodLog)
				{
					src = getTextureAtlas(/* left variant here */ 12, 0, 32, 32);

					DrawTexturePro(
						assetManager.textures, // or woodLog if separate atlas
						src,
						{ (float)x, (float)y, 1, 1 },
						{ 0, 0 },
						0.0f,
						WHITE
					);
				}
				else
				{
					src = getWoodLogTextureAtlas(/* bottom variant here */ 0, 0, 32, 16);
					DrawTexturePro(
						assetManager.woodLog, // or woodLog if separate atlas
						src,
						{ (float)x, (float)y, 1, 1 },
						{ 0, 0 },
						0.0f,
						WHITE
					);
				}
			}
		}

	// Draw selected block
	DrawTexturePro(
		assetManager.frame,
		{ 0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height}, //source
		{ (float)blockX, (float)blockY, 1, 1 }, //dest
		{ 0, 0 }, //origin (top-left corner)
		0.0f, //rotation
		WHITE //tint
	);

	EndMode2D();
#pragma endregion

	DrawFPS(10, 10);
	return true;
}

void closeGame()
{
	// Resets the game state
	gameData = {};
}
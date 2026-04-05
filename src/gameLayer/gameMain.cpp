#include <raylib.h>
#include "gameMain.h"
#include <asserts.h>
#include <assetManager.h>
#include <gameMap.h>
#include <helpers.h>
#include <raymath.h>
#include <vector>
#include <queue>
#include <worldGenerator.h>
#include <imgui.h>

struct GameData
{

	GameMap gameMap;
	Camera2D camera;
}gameData;

AssetManager assetManager;

bool isWoodLogAt(int x, int y);
std::uint16_t getDynamicTreeBlockToPlace(int x, int y);
Rectangle getWoodLogAutoTile(int x, int y);

std::uint16_t selectedBlockType = Block::woodPlank;

bool initGame()
{
	assetManager.loadAll();

	generateWorld(gameData.gameMap);

	gameData.camera.target = { 20, 120 }; // world-space center of view
	gameData.camera.rotation = 0.0f;
	gameData.camera.zoom = 100.0f;

	return true;
}

struct AutoTileResult
{
	Rectangle src;
	Texture2D* texture;
};

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5) { deltaTime = 1 / 5.f; }

	gameData.camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };

	ClearBackground({ 75, 75, 150, 255 });

#pragma region camera movement
	static float CAMERA_SPEED = 30.f;
	if (IsKeyDown(KEY_LEFT)) gameData.camera.target.x -= CAMERA_SPEED * deltaTime;
	if (IsKeyDown(KEY_RIGHT)) gameData.camera.target.x += CAMERA_SPEED * deltaTime;
	if (IsKeyDown(KEY_UP)) gameData.camera.target.y -= CAMERA_SPEED * deltaTime;
	if (IsKeyDown(KEY_DOWN)) gameData.camera.target.y += CAMERA_SPEED * deltaTime;
#pragma endregion

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);
	if (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		auto wall = gameData.gameMap.getWallSafe(blockX, blockY);
		if (wall)
		{
			*wall = 0;
		}
	}
	else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		auto block = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (block)
		{
			*block = {};
		}
	}

	if (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		auto wall = gameData.gameMap.getWallSafe(blockX, blockY);
		if (wall)
		{
			*wall = 1; // example wall id
		}
	}
	else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		auto block = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (block)
		{
			if (selectedBlockType == Block::woodLog)
			{
				block->type = getDynamicTreeBlockToPlace(blockX, blockY);
			}
			else
			{
				block->type = selectedBlockType;
			}
		}
	}

#pragma region block selection
	if (IsKeyPressed(KEY_F1))
	{
		selectedBlockType = Block::woodPlank;
	}
	else if (IsKeyPressed(KEY_F2))
	{
		selectedBlockType = Block::woodLog;
	}

#pragma endregion

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
			auto& tile = gameData.gameMap.getTileUnsafe(x, y);

			if (tile.wall != 0)
			{
				DrawTexturePro(
					assetManager.backGroundWall,
					getWallTextureAtlas(tile.wall),
					{ (float)x, (float)y, 1, 1 },
					{ 0, 0 },
					0.0f,
					WHITE
				);
			}

			if (tile.block.type != Block::air)
			{
				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(tile.block.type, 0, 32, 32), //source
					{ (float)x, (float)y, 1, 1}, //dest
					{ 0, 0 }, //origin (top-left corner)
					0.0f, //rotation
					WHITE //tint
				);
			}

			Rectangle src = getWoodLogTextureAtlas(tile.block.type, 0, 32, 16);
			if (tile.block.type == Block::woodLog)
			{
				DrawTexturePro(
					assetManager.woodLog,
					getWoodLogAutoTile(x, y),
					{ (float)x, (float)y, 1, 1 },
					{ 0, 0 },
					0.0f,
					WHITE
				);
			}
			else if (tile.block.type == Block::leaves)
			{
				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(tile.block.type, 0, 32, 32),
					{ (float)x, (float)y, 1, 1 },
					{ 0, 0 },
					0.0f,
					WHITE
				);
			}
			else
			{
				src = getTextureAtlas(tile.block.type, 0, 32, 32);

				DrawTexturePro(
					assetManager.textures,
					src,
					{ (float)x, (float)y, 1, 1 },
					{ 0, 0 },
					0.0f,
					WHITE
				);
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

	ImGui::Begin("Game control");

	ImGui::SliderFloat("Camera zoom", &gameData.camera.zoom, 1, 150);
	ImGui::SliderFloat("Camera speed", &CAMERA_SPEED, 5, 80);

	ImGui::End();

	DrawFPS(10, 10);
	DrawText(TextFormat("Selected block: %d", selectedBlockType), 10, 30, 20, WHITE);
	return true;
}

void closeGame()
{
	// Resets the game state
	gameData = {};
}

bool isWoodLogAt(int x, int y)
{
	auto block = gameData.gameMap.getBlockSafe(x, y);
	return block && block->type == Block::woodLog;
}

bool isLeavesAt(int x, int y)
{
	auto block = gameData.gameMap.getBlockSafe(x, y);
	return block && block->type == Block::leaves;
}

std::uint16_t getDynamicTreeBlockToPlace(int x, int y)
{
	bool woodUp = isWoodLogAt(x, y - 1);
	bool woodDown = isWoodLogAt(x, y + 1);
	bool woodLeft = isWoodLogAt(x - 1, y);
	bool woodRight = isWoodLogAt(x + 1, y);

	bool leafUp = isLeavesAt(x, y - 1);
	bool leafDown = isLeavesAt(x, y + 1);
	bool leafLeft = isLeavesAt(x - 1, y);
	bool leafRight = isLeavesAt(x + 1, y);

	// If we're touching any leaves, keep building canopy
	if (leafUp || leafDown || leafLeft || leafRight)
		return Block::leaves;

	// Sideways from trunk = leaves
	if (woodLeft || woodRight)
		return Block::leaves;

	// Vertical growth = trunk
	if (woodUp || woodDown)
		return Block::woodLog;

	return Block::woodLog;
}

Rectangle getWoodLogAutoTile(int x, int y)
{
	bool up = isWoodLogAt(x, y - 1);
	bool down = isWoodLogAt(x, y + 1);

	if (!up && down)
		return getWoodLogTextureAtlas(5, 0, 32, 16);

	if (up && down)
		return getWoodLogTextureAtlas(0, 0, 32, 16);

	if (up && !down)
		return getWoodLogTextureAtlas(4, 0, 32, 16);

	return getWoodLogTextureAtlas(7, 0, 32, 16);
}



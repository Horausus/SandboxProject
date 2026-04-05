#pragma once
#include <vector>
#include <blocks.h>
#include <walls.h>
#include <cstdint>

struct Tile
{
	Block block;
	std::uint16_t wall = Wall::none;
};

struct GameMap
{
	int w = 0;
	int h = 0;

	std::vector<Tile> mapData;

	void create(int w, int h);

	Tile& getTileUnsafe(int x, int y);
	Tile* getTileSafe(int x, int y);

	Block& getBlockUnsafe(int x, int y);
	Block* getBlockSafe(int x, int y);

	std::uint16_t& getWallUnsafe(int x, int y);
	std::uint16_t* getWallSafe(int x, int y);
};
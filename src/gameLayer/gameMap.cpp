#include "gameMap.h"
#include <asserts.h>

void GameMap::create(int w, int h)
{
	*this = {}; // reset all the data
	mapData.resize(w * h);

	this->w = w;
	this->h = h;

	for (auto& e : mapData)
	{
		e = {};
	}
}

Tile& GameMap::getTileUnsafe(int x, int y)
{
	permaAssertCommentDevelopement(
		mapData.size() == w * h,
		"Map data not initialized"
	);

	permaAssertCommentDevelopement(
		x >= 0 && y >= 0 && x < w && y < h,
		"getTileUnsafe out of bounds error"
	);

	return mapData[x + y * w];
}

Tile* GameMap::getTileSafe(int x, int y)
{
	permaAssertCommentDevelopement(
		mapData.size() == w * h,
		"Map data not initialized"
	);

	if (x < 0 || y < 0 || x >= w || y >= h)
	{
		return nullptr;
	}

	return &mapData[x + y * w];
}

Block& GameMap::getBlockUnsafe(int x, int y)
{
	return getTileUnsafe(x, y).block;
}

Block* GameMap::getBlockSafe(int x, int y)
{
	Tile* tile = getTileSafe(x, y);
	if (!tile)
	{
		return nullptr;
	}
	return &tile->block;
}

std::uint16_t& GameMap::getWallUnsafe(int x, int y)
{
	return getTileUnsafe(x, y).wall;
}

std::uint16_t* GameMap::getWallSafe(int x, int y)
{
	Tile* tile = getTileSafe(x, y);
	if (!tile)
	{
		return nullptr;
	}
	return &tile->wall;
}
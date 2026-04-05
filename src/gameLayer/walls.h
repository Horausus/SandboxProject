#pragma once
#include <cstdint>

struct Wall
{
	enum
	{
		none = 0,

		dirtWall,
		stoneWall,
		woodWall,
		sandStoneWall,
		brickWall,
		glassWall,
		copperBlockWall,
		silverBlockWall,
		goldBlockWall,
		snowWall,
		sandWall,
		stoneBricksWall,
		rubyBlockWall,
		heroglyphWall,
		blueRubyWall,
		plankedWall,
		snowBrickWall,
		boneBrickWall,

		WALLS_COUNT
	};
};
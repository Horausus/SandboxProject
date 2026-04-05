#pragma once
#include <walls.h>
#include <raylib.h>

Rectangle getTextureAtlas(int x, int y , int cellSizePixelsX, int cellSizePixelsY);
Rectangle getWoodLogTextureAtlas(int x, int y, int cellSizePixelsX, int cellSizePixelsY);

inline Rectangle getWallTextureAtlas(std::uint16_t wallType, int tileW = 32, int tileH = 32)
{
	int column = 0;
	int row = 0;

	switch (wallType)
	{
	case Wall::dirtWall:         column = 54;  row = 0; break;
	case Wall::stoneWall:        column = 55;  row = 0; break;
	case Wall::woodWall:         column = 56;  row = 0; break;
	case Wall::sandStoneWall:    column = 57;  row = 0; break;
	case Wall::brickWall:        column = 58;  row = 0; break;
	case Wall::glassWall:        column = 59;  row = 0; break;
	case Wall::copperBlockWall:  column = 60;  row = 0; break;
	case Wall::silverBlockWall:  column = 61;  row = 0; break;
	case Wall::goldBlockWall:    column = 62;  row = 0; break;
	case Wall::snowWall:         column = 63;  row = 0; break;
	case Wall::sandWall:         column = 64; row = 0; break;
	case Wall::stoneBricksWall:  column = 65; row = 0; break;
	case Wall::rubyBlockWall:    column = 66; row = 0; break;
	case Wall::heroglyphWall:    column = 67; row = 0; break;
	case Wall::blueRubyWall:     column = 68; row = 0; break;
	case Wall::plankedWall:      column = 69; row = 0; break;
	case Wall::snowBrickWall:    column = 70; row = 0; break;
	case Wall::boneBrickWall:    column = 71; row = 0; break;

	default:
		return Rectangle{ 0, 0, 0, 0 };
	}

	return Rectangle{
		(float)(column * tileW),
		(float)(row * tileH),
		(float)tileW,
		(float)tileH
	};
}
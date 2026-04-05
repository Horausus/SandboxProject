#include "worldGenerator.h"
#include "randomStuff.h"
#include <FastNoiseSIMD.h>
#include <algorithm>
#include <iostream>

void generateWorld(GameMap& gameMap, int seed)
{
	const int w = 900;
	const int h = 500;

	gameMap.create(w, h);

	int stoneSize = 380;
	int dirtSize = 50;

	std::ranlux24_base rng(seed++);

	int desertStart = getRandomInt(rng, 10, w - 210);
	int desertEnd = desertStart + 100 + getRandomInt(rng, 0, 100);
	if (desertEnd > w) { desertEnd = w; }

	std::unique_ptr<FastNoiseSIMD> dirtNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
	//std::unique_ptr<FastNoiseSIMD> stoneNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());

	dirtNoiseGenerator->SetSeed(seed++);
	//stoneNoiseGenerator->SetSeed(seed++);

	dirtNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	dirtNoiseGenerator->SetFractalOctaves(1);
	dirtNoiseGenerator->SetFrequency(0.02f);
	/*
	stoneNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	stoneNoiseGenerator->SetFractalOctaves(4);
	stoneNoiseGenerator->SetFrequency(0.01f);
	*/
	float *dirtNoise = FastNoiseSIMD::GetEmptySet(w);
	float *stoneNoise = FastNoiseSIMD::GetEmptySet(w);

	dirtNoiseGenerator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);
	//stoneNoiseGenerator->FillNoiseSet(stoneNoise, 0, 0, 0, w, 1, 1);

	//convert from range [-1, 1] to [0, 1]
	for (int i = 0; i < w; i++)
	{
		dirtNoise[i] = (dirtNoise[i] + 1) / 2; 
		stoneNoise[i] = (stoneNoise[i] + 1) / 2;

		//stoneNoise[i] = std::powf(stoneNoise[i], 2f); //steeper mountains
	}
	
	int dirtOffsetStart = -5;
	int dirtOffsetEnd = 35;
	/*
	int stoneHeightStart = 80;
	int stoneHeightEnd = 170;
	*/
	int keepDirectionTimeStone = getRandomInt(rng, 5, 40);
	int directionStone = getRandomInt(rng, -2, 2);

	int stoneHeight = 90;

	for (int x = 0; x < w; x++)
	{
		bool inDesert = (x >= desertStart && x <= desertEnd);
		#pragma region stone height
		
		keepDirectionTimeStone--;
		if (keepDirectionTimeStone <= 0)
		{
			keepDirectionTimeStone = getRandomInt(rng, 5, 40);
			directionStone = getRandomInt(rng, -2, 2);
		}

		if (directionStone == -1)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight--;
			}
		}		  
		else if (directionStone == -2)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight -= 2;
			}

			if (getRandomChance(rng, 0.25))
			{
				stoneHeight -= 2;
			}
		}
		else if (directionStone == 1)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight++;
			}
		}
		else if (directionStone == 2)
		{
			if (getRandomChance(rng, 0.25))
			{
				stoneHeight += 2;
			}

			if (getRandomChance(rng, 0.25))
			{
				stoneHeight += 2;
			}
		}

		if (stoneHeight < 60)
		{
			stoneHeight = 60;

		}
		
		if (stoneHeight > 120)
		{
			stoneHeight = 120;
		}
		#pragma endregion
		
		//int stoneHeight = stoneHeightStart + (stoneHeightEnd - stoneHeightStart) * stoneNoise[x];
		int dirtHeight = dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];
		dirtHeight = stoneHeight - dirtHeight;

		int dirtType = Block::dirt;
		int grassType = Block::grassBlock;
		int stoneType = Block::stone;

		if(inDesert)
		{ 
			dirtType = Block::sand;
			grassType = Block::sand;
			stoneType = Block::sandStone;
		}
		for (int y = 0; y < h; y++)
		{
			Block b;

			if (y > dirtHeight)
			{
				b.type = dirtType;
			}
			
			if (y == dirtHeight)
			{
				b.type = grassType;
			}
			
			if (y >= stoneHeight)
			{
				b.type = stoneType;
			}

			if (inDesert)
			{
				int desertMid = (desertStart + desertEnd) / 2;
				int desertHalfWidth = (desertEnd - desertStart) / 2;
				int distanceFromDesertMid = std::abs(x - desertMid);

				//This gives a value from 0 at edge to 1 at center
				float desertDistance = 1 - distanceFromDesertMid / float(desertHalfWidth);
				desertDistance = std::clamp(desertDistance, 0.0f, 1.0f);
				desertDistance = pow(desertDistance, 0.5);

				int desertStoneStart = 10 + stoneHeight;
				int desertStoneDepth = 20 + stoneHeight; // How deep the triangle goes

				int triangleStoneY = desertStoneStart + desertDistance * desertStoneDepth;

				// Apply stone if below the triangle
				if (y > triangleStoneY)
				{
					b.type = Block::stone;
				}
			}

			gameMap.getBlockUnsafe(x, y) = b;
		}
		

	}

	FastNoiseSIMD::FreeNoiseSet(dirtNoise);
	//FastNoiseSIMD::FreeNoiseSet(stoneNoise);
}
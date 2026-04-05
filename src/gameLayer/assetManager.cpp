#include "assetManager.h"

void AssetManager::loadAll()
{
	dirt = LoadTexture(RESOURCES_PATH "dirt.png");
	textures = LoadTexture(RESOURCES_PATH "textures.png");
	frame = LoadTexture(RESOURCES_PATH "frame.png");
	woodLog = LoadTexture(RESOURCES_PATH "treetextures.png");
	backGroundWall = LoadTexture(RESOURCES_PATH "texturesWithBackgroundVersion.png");
}
#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode)
{
	// Creates a CubeAsset in the asset manager
	asset_manager = std::make_shared<GameAssetManager>(mode);

	// Make a few cubes
	for(int i = 0; i < 5; i++)
	{
		asset_manager->AddAsset(std::make_shared<CubeAsset>(0.0 + i, 0.0 + i, 0.0 + i));
	}
}

void GameWorld::Draw()
{
	// Calls draw from the asset manager
	asset_manager->Draw();
}

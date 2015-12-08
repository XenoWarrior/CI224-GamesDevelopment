#include "GameWorld.h"

GameWorld::GameWorld (ApplicationMode mode)
{
	// Creates a CubeAsset in the asset manager
	asset_manager = std::make_shared<GameAssetManager>(mode);
	asset_manager->AddAsset(std::make_shared<CubeAsset>());
}

void GameWorld::Draw()
{
	// Calls draw from the asset manager
	asset_manager->Draw();
}

#include "Framework.h"
#include "TestScene.h"

TestScene::TestScene()
{
	map = new InstancingTileMap(L"Textures/Tile/tiles.png", Vector2(20, 1), 5);
}

TestScene::~TestScene()
{
	delete map;
}

void TestScene::Update()
{
}

void TestScene::Render()
{
	map->Render(false);
}

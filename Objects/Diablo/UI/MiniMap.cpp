#include "Framework.h"

MiniMap::MiniMap(Vector2 mapSize)
	:mapSize(mapSize), mapHalfSize(mapSize * 0.5f)
{
	miniMap = new Quad(mapSize, Vector2(0, 0) * 17);
	miniMap->UpdateWorld();
	miniMap->Position() = Vector2(WIN_WIDTH, WIN_HEIGHT) - mapSize * 0.025f;
	Texture* texture = Texture::Add(L"DiabloMap");
	miniMap->SetTexture(texture);
	miniMap->Scale() *= 0.05f;
	miniMap->GetColorBuffer()->Set(1, 1, 1, 0.8f);

	marker = new Quad(L"Textures/UI/playerMark.png");
	marker->SetParent(miniMap);
}

MiniMap::~MiniMap()
{
	delete miniMap;
	delete marker;
}

void MiniMap::Update()
{
	miniMap->UpdateWorld();

	marker->Position() = player->Position() - mapHalfSize;
	marker->UpdateWorld();
}

void MiniMap::Render()
{
	miniMap->Render();
	marker->Render();
}

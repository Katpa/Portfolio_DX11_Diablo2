#include "Framework.h"
#include "DiabloScene.h"

DiabloScene::DiabloScene()
{
	/// <summary>
	/// 플레이어를 astarPlus랑 연결해줘야함
	/// </summary>

	gameTileMap = new GameTileMap("TextData/diablo.map");
	gameTileMap->Position() = { 0, 0 };
	gameTileMap->Scale() = { 1.5f, 1.5f };
	gameTileMap->Update(isDebugMode);

	cursor = new Cursor();

	astar = new AStar(gameTileMap);
	astarPlus = new AStarPlus(gameTileMap);

	cursor->GetAStarPlus(astarPlus);
}

DiabloScene::~DiabloScene()
{
	delete gameTileMap;
	delete cursor;

	delete astar;
	delete astarPlus;
}

void DiabloScene::Update()
{
	TempTileMapControl();
	DebugControl();

	gameTileMap->Update(isDebugMode);
	cursor->Update();
	astarPlus->Update();
	//astar->Update();
}

void DiabloScene::Render()
{
	gameTileMap->Render(isDebugMode);
	cursor->Render();
	if(isShowPath)
		astarPlus->Render();
}

void DiabloScene::DebugControl()
{
	if (KEY_DOWN('M'))
		isDebugMode = !isDebugMode;
	if (KEY_DOWN(VK_END))
		isShowPath = !isShowPath;
}

void DiabloScene::TempTileMapControl()
{
	if (KEY_PRESS(VK_UP))
		gameTileMap->Position().y -= 1000 * DELTA;
	else if (KEY_PRESS(VK_DOWN))
		gameTileMap->Position().y += 1000 * DELTA;
	if (KEY_PRESS(VK_RIGHT))
		gameTileMap->Position().x -= 1000 * DELTA;
	else if (KEY_PRESS(VK_LEFT))
		gameTileMap->Position().x += 1000 * DELTA;
}

#include "Framework.h"
#include "TankScene.h"

TankScene::TankScene()
{
	gameTileMap = new GameTileMap("TextData/Stage1.map");
	gameTileMap->Position() = { 50, 50 };
	gameTileMap->Update(false);

	tank = new Tank();
	tank->Position() = { 150, 150 };
	tank->Update();

	aStar = new AStar(gameTileMap);
}

TankScene::~TankScene()
{
	delete gameTileMap;
	delete tank;
	delete aStar;
}

void TankScene::Update()
{
	//gameTileMap->PushTank(tank);
	if (KEY_DOWN(VK_LBUTTON))
	{
		int start = aStar->FindCloseNode(tank->Position());
		int end = aStar->FindCloseNode(mousePos);
		
		aStar->GetPath(start, end, tank->GetPath());	

		tank->GetPath().insert(tank->GetPath().begin(), mousePos);
	}

	gameTileMap->Update(false);
	tank->Update();

	aStar->Update();
}

void TankScene::Render()
{
	gameTileMap->Render(false);
	tank->Render();
	aStar->Render();
}

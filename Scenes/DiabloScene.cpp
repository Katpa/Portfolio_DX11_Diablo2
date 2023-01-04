#include "Framework.h"
#include "DiabloScene.h"

DiabloScene::DiabloScene()
{
	map = new InstancingMap("TextData/Diablo.map");

	player = new Playable(DataManager::Get()->GetCharacterData("SO"));
	player->Position() = { 4040, 3980 };
	CAM->SetTarget(player);

	cain = new Cain();
	cain->Position() = { 4350, 4000 };

	astar = new AStar(map);
	astarPlus = new AStarPlus(map);
	cursor = new Cursor();
	cursor->GetAStarPlus(astarPlus);
	cursor->GetPlayable(player);
	cursor->GetUIManager(player->ReturnUIManager());
	cursor->GetInventory(player->ReturnInventory());
	cursor->GetCain(cain);
	cursor->GetConverter(player->ReturnConverter());

	SKILL->SetPlayer(player);
	SKILL->GetAStarPlus(astarPlus);

	MonsterManager::Get()->SetMonster(player, astar);

	miniMap = new MiniMap(map->ReturnMapSize());
	miniMap->GetCharacter(player);

	MonsterManager::Get()->StartMonsterSpawn();

	InitFX();
	SetSFX();

	SFX->Play("tristram", 0.05f);
}

DiabloScene::~DiabloScene()
{
	delete cursor;
	delete astar;
	delete astarPlus;
	delete player;
	delete map;
	delete cain;
	delete miniMap;
}

void DiabloScene::Update()
{
	DebugControl();

	cursor->Update();

	player->Update();

	cain->Update();

	MonsterManager::Get()->Update();

	SKILL->Update();

	map->Update();

	if(isMiniMap)
		miniMap->Update();
}

void DiabloScene::Render()
{
	map->Render(isDebugMode);

	ZOrder::Get()->Render();

	if(isShowPath)
		astarPlus->Render();
}

void DiabloScene::PostRender()
{
	player->RenderUI();
	cain->RenderUI();
	MonsterManager::Get()->RenderUI();

	if(isMiniMap)
		miniMap->Render();

	cursor->Render();
}

void DiabloScene::DebugControl()
{
	if (KEY_DOWN('M'))
		isDebugMode = !isDebugMode;
	if (KEY_DOWN(VK_END))
		isShowPath = !isShowPath;
	if (KEY_DOWN(VK_TAB))
		isMiniMap = !isMiniMap;
}

void DiabloScene::InitFX()
{
	FX->Add("iceCast", 5, L"Textures/Effects/iceCast1.png", Vector2(16, 1), 0.035f, true);
	FX->Add("fireCast", 5, L"Textures/Effects/fireCast.png", Vector2(14, 1), 0.035f, true);
	FX->Add("iceBolt", 100, L"Textures/Effects/iceExplosion.png", Vector2(16, 1), 0.05f, true);
	FX->Add("iceOrb", 5, L"Textures/Effects/iceOrbExplosion.png", Vector2(18, 1), 0.05f, true);
	FX->Add("fireBall", 10, L"Textures/Effects/fireExplosion.png", Vector2(16, 1), 0.05f, true);
}

void DiabloScene::SetSFX()
{
	SFX->Add("tristram", "Sounds/bgm/Tristram.mp3", true, true);
	SFX->Add("mesa", "Sounds/bgm/Mesa.mp3", true, true);

	SFX->Add("diabloIntro", "Sounds/diablo/laugh1.wav");
	SFX->Add("diabloAtk1", "Sounds/diablo/attack1.wav");
	SFX->Add("diabloAtk2", "Sounds/diablo/attack2.wav");
	SFX->Add("diabloCastFire", "Sounds/diablo/castfire.wav");
	SFX->Add("diabloCastLazer", "Sounds/diablo/castlazer.wav");
	SFX->Add("diabloDeath", "Sounds/diablo/death.wav");

	SFX->Add("fireCast", "Sounds/so/fireCast.wav");
	SFX->Add("teleport", "Sounds/so/teleport.wav");
	SFX->Add("iceCast", "Sounds/so/coldCast.wav");
	SFX->Add("iceOrb", "Sounds/so/iceOrb.mp3");
	SFX->Add("cantUseYet", "Sounds/so/Sor_cantuseyet.wav");
	
	SFX->Add("cainHello", "Sounds/cain/hello.wav");
	SFX->Add("cainGoodbye", "Sounds/cain/goodbye.wav");

	SFX->Add("gold", "Sounds/miscellaneous/gold.wav");
	SFX->Add("levelup", "Sounds/miscellaneous/levelup.wav");
	SFX->Add("potion", "Sounds/miscellaneous/potiondrink.wav");
	SFX->Add("convert", "Sounds/miscellaneous/convert.wav");
	SFX->Add("button", "Sounds/miscellaneous/button.wav");
}

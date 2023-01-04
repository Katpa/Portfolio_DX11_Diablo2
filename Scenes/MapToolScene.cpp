#include "Framework.h"
#include "MapToolScene.h"

MapToolScene::MapToolScene()
{
	map = new InstancingTileMap(L"Textures/Tile/tiles.png", Vector2(20, 1), 35);

	CAM->SetLeftBottom(0, 0);
	CAM->SetRightTop(map->ReturnMapSize());

	pallet = new TilePallet(L"Textures/Tile/tiles_pallet.png", Vector2(20, 1));
	pallet->SetSelectedTileNum(map->ReturnSelectedTileNum());
	pallet->SetSelectedObj(map->ReturnSelectedObj());

	char path[128];
	GetCurrentDirectoryA(sizeof(path), path);
	projectPath = path;
}

MapToolScene::~MapToolScene()
{
	delete map;
	delete pallet;
}

void MapToolScene::Update()
{
	map->Update();
	pallet->Update();
}

void MapToolScene::Render()
{
	map->Render(false);
	pallet->Render(false);
}

void MapToolScene::PostRender()
{
	Save();
	Load();
}

void MapToolScene::Save()
{
	if (ImGui::Button("Save"))
		ImGuiFileDialog::Instance()->OpenDialog("Save", "SaveFile", ".map", ".");

	if (ImGuiFileDialog::Instance()->Display("Save"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();
			file = file.substr(projectPath.size() + 1, file.length());

			map->Save(file);
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void MapToolScene::Load()
{
	if (ImGui::Button("Load"))
		ImGuiFileDialog::Instance()->OpenDialog("Load", "LoadFile", ".map", ".");

	if (ImGuiFileDialog::Instance()->Display("Load"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();
			file = file.substr(projectPath.size() + 1, file.length());

			map->Load(file);
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

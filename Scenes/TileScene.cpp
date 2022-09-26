#include "Framework.h"
#include "TileScene.h"

TileScene::TileScene()
{
	CreateSample();

	tileMap = new TileMap(30, 30);
	tileMap->Position() = { 0, 0 };

	char path[128];
	GetCurrentDirectoryA(sizeof(path), path);
	projectPath = path;
}

TileScene::~TileScene()
{
	for (Button* btn : sampleBtns)
		delete btn;

	delete tileMap;
}

void TileScene::Update()
{
	if (KEY_PRESS(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		tileMap->ClickTile(sampleFiles[selectSampleNum], selectType);
	}

	if (KEY_PRESS(VK_RBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		tileMap->DeleteTile();
	}

	for (Button* btn : sampleBtns)
		btn->Update();

	if (KEY_PRESS('A'))
		tileMap->Position().x -= 500 * DELTA;
	else if (KEY_PRESS('D'))
		tileMap->Position().x += 500 * DELTA;
	if (KEY_PRESS('S'))
		tileMap->Position().y -= 500 * DELTA;
	else if (KEY_PRESS('W'))
		tileMap->Position().y += 500 * DELTA;

	if (KEY_DOWN('M'))
		isDebugMode = !isDebugMode;

	tileMap->Update();
}

void TileScene::Render()
{
	tileMap->Render(isDebugMode);

	for (Button* btn : sampleBtns)
		btn->Render();
}

void TileScene::PostRender()
{
	ImGui::Text("SelectNum : %d", selectSampleNum);

	const char* list[] = { "BG", "OBJ" };
	ImGui::ListBox("Type", (int*)&selectType, list, 2);
	
	Save();
	Load();
}

void TileScene::CreateSample()
{
	WIN32_FIND_DATA findData;

	HANDLE handle;
	bool result = true;
	handle = FindFirstFile(L"Textures/Tile\\*.png", &findData);

	wstring fileName;

	UINT width = 18;
	Vector2 size(40, 40);

	Vector2 startPos(WIN_WIDTH - size.x, WIN_HEIGHT - size.y);

	while (result)
	{
		fileName = L"Textures/Tile/";
		fileName += findData.cFileName;		
		
		UINT index = sampleBtns.size();

		Vector2 pos(index % width, index / width);

		Button* button = new Button(size);
		button->SetTexture(Texture::Add(fileName));
		button->Position() = startPos - (pos * size);
		button->SetIntParam(index);		
		button->SetIntEvent(bind(&TileScene::ClickSample, this, placeholders::_1));

		sampleBtns.push_back(button);
		sampleFiles.push_back(fileName);

		result = FindNextFile(handle, &findData);
	}
}

void TileScene::ClickSample(int index)
{
	selectSampleNum = index;
}

void TileScene::Save()
{
	if (ImGui::Button("Save"))
		ImGuiFileDialog::Instance()->OpenDialog("Save", "SaveFile", ".map", ".");

	if (ImGuiFileDialog::Instance()->Display("Save"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();
			file = file.substr(projectPath.size() + 1, file.length());

			tileMap->Save(file);
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void TileScene::Load()
{
	if (ImGui::Button("Load"))
		ImGuiFileDialog::Instance()->OpenDialog("Load", "LoadFile", ".map", ".");

	if (ImGuiFileDialog::Instance()->Display("Load"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			string file = ImGuiFileDialog::Instance()->GetFilePathName();
			file = file.substr(projectPath.size() + 1, file.length());

			tileMap->Load(file);
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

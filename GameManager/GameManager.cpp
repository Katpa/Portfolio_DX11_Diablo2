#include "Framework.h"

#include "Scenes/MapToolScene.h"
#include "Scenes/DiabloScene.h"

GameManager::GameManager()
{
	Create();

	//scene = new MapToolScene();
	scene = new DiabloScene();
}

GameManager::~GameManager()
{
	delete scene;

	Delete();
}

void GameManager::Update()
{
	Keyboard::Get()->Update();
	Timer::Get()->Update();
	FX->Update();
	Audio::Get()->Update();

	scene->Update();

	CAM->Update();
}

void GameManager::Render()
{
	renderFrame += DELTA;

	if (renderFrame > renderDelay)
	{
		renderFrame -= renderDelay;

		scene->PreRender();

		Device::Get()->Clear();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Font::Get()->GetDC()->BeginDraw();

		string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
		Font::Get()->RenderText(fps, "default", Vector2(80, WIN_HEIGHT - 20));

		Device::Get()->SetRenderTarget();
		Environment::Get()->SetViewPort();
		Environment::Get()->SetOrtographic();

		scene->Render();
		FX->Render();

		//CAM->RenderUI();
		Environment::Get()->SetUIView();
		scene->PostRender();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		Font::Get()->GetDC()->EndDraw();

		Device::Get()->Present();
	}
}

void GameManager::Create()
{
	Device::Get();
	Keyboard::Get();
	Timer::Get();
	Environment::Get();
	EffectManager::Get();
	Audio::Get();
	DataManager::Get();
	ItemManager::Get();
	SkillManager::Get();
	MonsterManager::Get();
	
	Font::Get()->Add("default", L"kodia");

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete()
{
	Device::Delete();
	Keyboard::Delete();
	Timer::Delete();
	Environment::Delete();
	EffectManager::Delete();
	Audio::Delete();
	DataManager::Delete();
	ItemManager::Delete();
	Font::Delete();
	SkillManager::Delete();
	MonsterManager::Delete();

	Texture::Delete();
	Shader::Delete();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

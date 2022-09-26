#include "Framework.h"

UIManager::UIManager(UINT* strength, UINT* dexterlity, UINT* energy, UINT* viality,
					int* curHP, int* curMp, int* curSP, int* curEXP)
	:strength(strength), dexterlity(dexterlity), energy(energy), viality(viality),
	curHP(curHP), curMP(curMP), curSP(curSP), curEXP(curEXP)
{
	basicUI = new Quad(L"Textures/UI/basicUI.png");
	basicUI->Position() = { CENTER_X, 27 };
	//basicUI->SetParent(CAM);

	hpBarBG = new Quad(L"Textures/UI/hpBG.png");
	hpBarBG->Position().x = CENTER_X - (basicUI->Size().x + hpBarBG->Size().x) * 0.5f;
	hpBarBG->Position().y = hpBarBG->Size().y * 0.5f;
	//hpBarBG->SetParent(CAM);

	mpBarBG = new Quad(L"Textures/UI/mpBG.png");
	mpBarBG->Position().x = CENTER_X + (basicUI->Size().x + mpBarBG->Size().x) * 0.5f;
	mpBarBG->Position().y = mpBarBG->Size().y * 0.5f;
	//mpBarBG->SetParent(CAM);


}

UIManager::~UIManager()
{
	delete basicUI;
	delete hpBarBG;
	delete mpBarBG;

	delete HPBar;
	delete MPBar;
	delete SPBar;
	delete EXPBar;
}

void UIManager::Update()
{
}

void UIManager::Render(bool isDebugMode)
{
}

void UIManager::OpenStatus()
{
}

void UIManager::CloseState()
{

}

void UIManager::PlusStat()
{

}

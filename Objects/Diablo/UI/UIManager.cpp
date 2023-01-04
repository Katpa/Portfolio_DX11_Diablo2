#include "Framework.h"

UIManager::UIManager(UINT* strength, UINT* dexterlity, UINT* energy, UINT* viality,
					int* curHP, int* curMP, float* curSP, int* curEXP,
					UINT* maxHP, UINT* maxMP, UINT* maxSP, UINT* maxEXP, UINT* level,
					bool* isRun, int* statPoint, UINT* dmg, UINT* def)
	:strength(strength), dexterlity(dexterlity), energy(energy), viality(viality),
	curHP(curHP), curMP(curMP), curSP(curSP), curEXP(curEXP),
	maxHP(maxHP), maxMP(maxMP), maxSP(maxSP), maxEXP(maxEXP), level(level), isRun(isRun), statPoint(statPoint),
	dmg(dmg), def(def)
{
	status.Init(statPoint);
	InitStatus();

	basicUI = new Quad(L"Textures/UI/basicUI.png");
	basicUI->Position() = { CENTER_X, basicUI->Size().y * 0.5f * TEX_SCALE_MULTI };
	basicUI->Scale() *= TEX_SCALE_MULTI;
	basicUI->SetParent(this);

	leftSkill = new Quad(L"Textures/UI/SI_basic.png");
	leftSkill->Position().x = CENTER_X - (basicUI->Size().x * 0.5f + leftSkill->Size().x * 0.5f) * TEX_SCALE_MULTI;
	leftSkill->Position().y = leftSkill->Size().y * 0.5f * TEX_SCALE_MULTI;
	leftSkill->Scale() *= TEX_SCALE_MULTI;
	rightSkill = new Quad(L"Textures/UI/SI_fireball.png");
	rightSkill->Position().x = CENTER_X + (basicUI->Size().x * 0.5f + leftSkill->Size().x * 0.5f) * TEX_SCALE_MULTI;
	rightSkill->Position().y = rightSkill->Size().y * 0.5f * TEX_SCALE_MULTI;
	rightSkill->Scale() *= TEX_SCALE_MULTI;
	coolTimeOverlay = new Quad(L"Textures/UI/coolTime.png");
	coolTimeOverlay->SetParent(rightSkill);
	coolTimeOverlay->Active() = false;

	hpBarBG = new Quad(L"Textures/UI/hpBG.png");
	hpBarBG->Position().x = leftSkill->Position().x - ((leftSkill->Size().x + hpBarBG->Size().x) * 0.5f - 17) * TEX_SCALE_MULTI;
	hpBarBG->Position().y = hpBarBG->Size().y * 0.5f * TEX_SCALE_MULTI;
	hpBarBG->Scale() *= TEX_SCALE_MULTI;
	hpBarBG->SetParent(this);

	mpBarBG = new Quad(L"Textures/UI/mpBG.png");
	mpBarBG->Position().x = rightSkill->Position().x + ((rightSkill->Size().x + mpBarBG->Size().x) * 0.5f - 17) * TEX_SCALE_MULTI;
	mpBarBG->Position().y = mpBarBG->Size().y * 0.5f * TEX_SCALE_MULTI;
	mpBarBG->Scale() *= TEX_SCALE_MULTI;
	mpBarBG->SetParent(this);

	HPBar = new VerticalProgressBar(L"Textures/UI/progressHP.png", curHP, maxHP);
	HPBar->SetParent(hpBarBG);
	MPBar = new VerticalProgressBar(L"Textures/UI/progressMP.png", curMP, maxMP);
	MPBar->SetParent(mpBarBG);
	EXPBar = new HorizentalProgressBar(L"Textures/UI/progressEXP.png", curEXP, maxEXP);
	EXPBar->SetParent(basicUI);
	EXPBar->Position() -= Vector2(84, -11);
	SPBar = new HorizentalProgressBar(L"Textures/UI/progressSP.png", curSP, maxSP);
	SPBar->SetParent(basicUI);
	SPBar->Position() -= Vector2(76, 8);

	Vector2 colliderSize = { 950, 75 };
	basicUICollider = new RectCollider(colliderSize);
	basicUICollider->Position() = basicUI->Position();
	basicUICollider->UpdateWorld();

	btnWalk = new UpDownButton(L"Textures/UI/btn_walk_up.png", L"Textures/UI/btn_walk_down.png");
	btnWalk->SetEvent(bind(&UIManager::SwitchRunBtn, this));
	btnWalk->Position() = basicUI->Position() - Vector2(205, 12);
	btnWalk->Scale() *= TEX_SCALE_MULTI;
	btnRun = new UpDownButton(L"Textures/UI/btn_run_up.png", L"Textures/UI/btn_run_down.png");
	btnRun->SetEvent(bind(&UIManager::SwitchRunBtn, this));
	btnRun->Position() = basicUI->Position() - Vector2(205, 12);
	btnRun->Scale() *= TEX_SCALE_MULTI;

	skillList["fireBall"] = Texture::Add(L"Textures/UI/SI_fireball.png");
	skillList["teleport"] = Texture::Add(L"Textures/UI/SI_teleport.png");
	skillList["iceOrb"] = Texture::Add(L"Textures/UI/SI_iceorb.png");
	skillList["fanaticism"] = Texture::Add(L"Textures/UI/SI_fanaticism.png");

	Font::Get()->Add("status", L"kodia", Float3(1, 1, 1), 15.0f);
	Font::Get()->GetFormat("status")->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
}

UIManager::~UIManager()
{
	delete basicUI;
	delete hpBarBG;
	delete mpBarBG;
	status.Delete();
	delete HPBar;
	delete MPBar;
	delete SPBar;
	delete EXPBar;
	delete btnWalk;
	delete btnRun;
	delete leftSkill;
	delete rightSkill;
	delete coolTimeOverlay;
	delete basicUICollider;
}

void UIManager::Update()
{
	SetIsMouseOnStatus();

	UpdateWorld();
	basicUI->UpdateWorld();
	leftSkill->UpdateWorld();
	rightSkill->UpdateWorld();
	coolTimeOverlay->UpdateWorld();
	hpBarBG->UpdateWorld();
	HPBar->Update();
	mpBarBG->UpdateWorld();
	MPBar->Update();

	EXPBar->Update();
	SPBar->Update();

	if(*isRun)
		btnRun->Update();
	else
		btnWalk->Update();
	
	status.Update();
	status.SetStatString((int)*strength, (int)*dexterlity, (int)*energy, (int)*viality,
						 (int)*maxHP, (int)*maxMP, (int)*maxSP, (int)*maxEXP,
						 (int)*curHP, (int)*curMP, (int)*curSP, (int)*curEXP,
						 (int)*level, (int)*dmg, (int)*def);
}

void UIManager::Render(bool isDebugMode)
{
	basicUI->Render();
	leftSkill->Render();
	rightSkill->Render();

	if(coolTimeOverlay->Active())
		coolTimeOverlay->Render();

	hpBarBG->Render();
	HPBar->Render();
	mpBarBG->Render();
	MPBar->Render();

	EXPBar->Render();
	SPBar->Render();

	if (*isRun)
		btnRun->Render();
	else
		btnWalk->Render();

	status.Render();
}

void UIManager::SetSkillIcon(string skillName)
{
	rightSkill->SetTexture(skillList[skillName]);
}

bool UIManager::IsMouseOnBasicUI()
{
	return basicUICollider->IsPointCollision(mousePos);
}

void UIManager::InitStatus()
{
	status.btnSTR->SetEvent(bind(&UIManager::ImproveSTR, this));
	status.btnDEX->SetEvent(bind(&UIManager::ImproveDEX, this));
	status.btnENG->SetEvent(bind(&UIManager::ImproveENG, this));
	status.btnVIA->SetEvent(bind(&UIManager::ImproveVIA, this));
	status.btnClose->SetEvent(bind(&UIManager::SwitchStatus, this));
}

void UIManager::SetIsMouseOnStatus()
{
	status.isMouseOn = false;

	if (!status.isActive) return;

	Vector2 TopRight = status.window->Position() + status.window->Size() * 0.5f;
	Vector2 BotLeft = status.window->Position() - status.window->Size() * 0.5f;

	if (mousePos.x < TopRight.x && mousePos.y < TopRight.y &&
		mousePos.x > BotLeft.x && mousePos.y > BotLeft.y)
	{
		status.isMouseOn = true;
	}
}

void UIManager::ImproveSTR()
{
	*strength += 1;
	int tmp = *statPoint;
	*statPoint = tmp - 1;
}

void UIManager::ImproveDEX()
{
	*dexterlity += 1;
	int tmp = *statPoint;
	*statPoint = tmp - 1;
}

void UIManager::ImproveENG()
{
	*energy += 1;
	int tmp = *statPoint;
	*statPoint = tmp - 1;
}

void UIManager::ImproveVIA()
{
	*viality += 1;
	int tmp = *statPoint;
	*statPoint = tmp - 1;
}
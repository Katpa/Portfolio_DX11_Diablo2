#pragma once

class UIManager: public Transform
{
private:
	struct Status
	{
		Quad* window;
		UpDownButton* btnSTR;
		UpDownButton* btnDEX;
		UpDownButton* btnENG;
		UpDownButton* btnVIA;
		UpDownButton* btnClose;
		Quad* statPointWin;

		int* statPoint;

		string strength;
		string dexterlity;
		string energy;
		string viality;
		string maxHP;
		string curHP;
		string maxMP;
		string curMP;
		string maxSP;
		string curSP;
		string maxEXP;
		string curEXP;
		string def;
		string dmg;
		string level;
		string statPointStr;

		bool isMouseOn;
		bool isActive = false;

		void Init(int* _statPoint)
		{
			window = new Quad(L"Textures/UI/status.png");
			btnSTR = new UpDownButton(L"Textures/UI/btn_levelup_up.png", L"Textures/UI/btn_levelup_down.png");
			btnDEX = new UpDownButton(L"Textures/UI/btn_levelup_up.png", L"Textures/UI/btn_levelup_down.png");
			btnENG = new UpDownButton(L"Textures/UI/btn_levelup_up.png", L"Textures/UI/btn_levelup_down.png");
			btnVIA = new UpDownButton(L"Textures/UI/btn_levelup_up.png", L"Textures/UI/btn_levelup_down.png");
			btnClose = new UpDownButton(L"Textures/UI/btn_cancel_up.png", L"Textures/UI/btn_cancel_down.png");
			statPointWin = new Quad(L"Textures/UI/statPointWin.png");

			window->Position() = { CENTER_X - window->Size().x * 0.75f, CENTER_Y * 1.1f };
			btnSTR->SetParent(window);
			btnSTR->Position() = { -17,124 };
			btnDEX->SetParent(window);
			btnDEX->Position() = { -17, 62 };
			btnVIA->SetParent(window);
			btnVIA->Position() = { -17, -24 };
			btnENG->SetParent(window);
			btnENG->Position() = { -17, -86 };
			btnClose->SetParent(window);
			btnClose->Position() = { -15, -189 };
			statPointWin->SetParent(window);
			statPointWin->Position() = { -90, -140 };

			this->statPoint = _statPoint;
		}
		void Update()
		{
			if (!isActive) return;
			
			window->UpdateWorld();
			
			btnClose->Update();
	
			if (*statPoint > 0)
			{
				btnSTR->Update();
				btnDEX->Update();
				btnENG->Update();
				btnVIA->Update();
				statPointWin->UpdateWorld();
			}
		}
		void Render()
		{
			if (!isActive) return;

			window->Render();

			Font::Get()->RenderText(level, "status", btnSTR->GlobalPosition() + Vector2(-112, 42));
			Font::Get()->RenderText(curEXP, "status", btnSTR->GlobalPosition() + Vector2(-20, 42));
			Font::Get()->RenderText(maxEXP, "status", btnSTR->GlobalPosition() + Vector2(105, 42));

			if (*statPoint > 0)
			{
				btnSTR->Render();

				btnDEX->Render();

				btnVIA->Render();

				btnENG->Render();

				statPointWin->Render();
				Font::Get()->RenderText("포인트", "status", statPointWin->GlobalPosition() - Vector2(20, 0));
				Font::Get()->RenderText(statPointStr, "status", statPointWin->GlobalPosition() + Vector2(40, 0));
			}

			Font::Get()->RenderText("힘", "status", btnSTR->GlobalPosition() - Vector2(100, 0));
			Font::Get()->RenderText(strength, "status", btnSTR->GlobalPosition() - Vector2(47, 0));
			Font::Get()->RenderText("공격력", "status", btnSTR->GlobalPosition() + Vector2(75, 0));
			Font::Get()->RenderText(dmg, "status", btnSTR->GlobalPosition() + Vector2(148, 0));

			Font::Get()->RenderText("민첩", "status", btnDEX->GlobalPosition() - Vector2(100, 0));
			Font::Get()->RenderText(dexterlity, "status", btnDEX->GlobalPosition() - Vector2(47, 0));
			Font::Get()->RenderText("방어력", "status", btnDEX->GlobalPosition() + Vector2(75, 0));
			Font::Get()->RenderText(def, "status", btnDEX->GlobalPosition() + Vector2(148, 0));

			Font::Get()->RenderText("생명력", "status", btnVIA->GlobalPosition() - Vector2(100, 0));
			Font::Get()->RenderText(viality, "status", btnVIA->GlobalPosition() - Vector2(47, 0));
			Font::Get()->RenderText("라이프", "status", btnVIA->GlobalPosition() + Vector2(50, 0));
			Font::Get()->RenderText(curHP, "status", btnVIA->GlobalPosition() + Vector2(110, 0));
			Font::Get()->RenderText(maxHP, "status", btnVIA->GlobalPosition() + Vector2(148, 0));
			Font::Get()->RenderText("스테미나", "status", btnVIA->GlobalPosition() + Vector2(50, -24));
			Font::Get()->RenderText(curSP, "status", btnVIA->GlobalPosition() + Vector2(110, -24));
			Font::Get()->RenderText(maxSP, "status", btnVIA->GlobalPosition() + Vector2(148, -24));

			Font::Get()->RenderText("에너지", "status", btnENG->GlobalPosition() - Vector2(100, 0));
			Font::Get()->RenderText(energy, "status", btnENG->GlobalPosition() - Vector2(47, 0));
			Font::Get()->RenderText("마나", "status", btnENG->GlobalPosition() + Vector2(50, 0));
			Font::Get()->RenderText(curMP, "status", btnENG->GlobalPosition() + Vector2(110, 0));
			Font::Get()->RenderText(maxMP, "status", btnENG->GlobalPosition() + Vector2(148, 0));

			btnClose->Render();
		}

		void Delete()
		{
			delete window;
			delete btnSTR;
			delete btnDEX;
			delete btnENG;
			delete btnVIA;
			delete btnClose;
			delete statPointWin;
		}

		void SetStatString(int str, int dex, int eng, int via,
						   int _maxHP, int _maxMP, int _maxSP, int _maxEXP,
						   int _curHP, int _curMP, int _curSP, int _curEXP,
						   int _level, int _dmg, int _def)
		{
			if (!isActive) return;

			strength = to_string(str);
			dexterlity = to_string(dex);
			energy = to_string(eng);
			viality = to_string(via);

			maxHP = to_string(_maxHP);
			curHP = to_string(_curHP);
			maxSP = to_string(_maxSP);
			curSP = to_string(_curSP);
			maxMP = to_string(_maxMP);
			curMP = to_string(_curMP);
			def = to_string(_def);
			dmg = to_string(_dmg);
			maxEXP = "다음 레벨\n" + to_string(_maxEXP);
			curEXP = "현재 경험치\n" + to_string(_curEXP);
			level = "레벨\n" + to_string(_level);

			if (*statPoint > 0)
				statPointStr = to_string(*statPoint);
		}
	};

public:
	UIManager(UINT* strength, UINT* dexterlity, UINT* energy, UINT* viality, int* curHP, int* curMp, float* curSP, int* curEXP,
			  UINT* maxHP, UINT* maxMP, UINT* maxSP, UINT* maxEXP, UINT* level, bool* isRun, int* statPoint, UINT* dmg, UINT* def);
	~UIManager();

	void Update();
	void Render(bool isDebugMode = false);

	void SetSkillIcon(string skillName);

	void SwitchCoolTime() { coolTimeOverlay->Active() = !coolTimeOverlay->Active(); }
	void SwitchStatus() { status.isActive = !status.isActive; }

	bool IsMouseOnStatus() { return status.isMouseOn; }
	bool IsMouseOnBasicUI();

private:
	void InitStatus();

	void SetIsMouseOnStatus();

	void SwitchRunBtn() { *isRun = !*isRun; }

	void ImproveSTR();
	void ImproveDEX();
	void ImproveENG();
	void ImproveVIA();

private:
	Status status;

	UINT* strength;
	UINT* dexterlity;
	UINT* energy;
	UINT* viality;

	int* curHP;
	int* curMP;
	float* curSP;
	int* curEXP;

	UINT* maxHP;
	UINT* maxMP;
	UINT* maxSP;
	UINT* maxEXP;

	UINT* level;
	int* statPoint;
	UINT* dmg;
	UINT* def;

	bool* isRun;

	UpDownButton* btnCloss;
	UpDownButton* btnStatus;
	
	RectCollider* basicUICollider;
	Quad* basicUI;
	Quad* hpBarBG;
	Quad* mpBarBG;

	VerticalProgressBar* HPBar;
	VerticalProgressBar* MPBar;
	HorizentalProgressBar* EXPBar;
	HorizentalProgressBar* SPBar;

	UpDownButton* btnRun;
	UpDownButton* btnWalk;

	//스킬 아이콘 이미지를 어떻게 넣어야 할까
	Quad* leftSkill;
	Quad* rightSkill;
	Quad* coolTimeOverlay;
	map<string, Texture*> skillList;
};
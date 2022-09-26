#pragma once

class UIManager
{
private:
	struct Status
	{
		Quad* window;
		Button* btnSTR;
		Button* btnDEX;
		Button* btnENG;
		Button* btnVIA;
		Button* btnClose;

		void Init()
		{
			window = new Quad(L"Textures/UI/status.png");
			btnSTR;
			btnDEX;
			btnENG;
			btnVIA;
			btnClose;

			btnSTR->SetParent(window);
			btnDEX->SetParent(window);
			btnENG->SetParent(window);
			btnVIA->SetParent(window);
			btnClose->SetParent(window);
		}
		void Update()
		{
			window->UpdateWorld();
			btnSTR->Update();
			btnDEX->Update();
			btnENG->Update();
			btnVIA->Update();
		}
		void Render()
		{
			window->Render();
			btnSTR->Render();
			btnDEX->Render();
			btnENG->Render();
			btnVIA->Render();
			btnClose->Render();
		}
	};
	struct UI
	{
		//하단 UI 구현
	};

public:
	UIManager(UINT* strength, UINT* dexterlity, UINT* energy, UINT* viality, int* curHP, int* curMp, int* curSP, int* curEXP);
	~UIManager();

	void Update();
	void Render(bool isDebugMode);

	void OpenStatus();

	bool IsStatusOpen() { return isStatusOpen; }

private:
	void CloseState();

	void PlusStat();

private:
	UINT* strength;
	UINT* dexterlity;
	UINT* energy;
	UINT* viality;

	int* curHP;
	int* curMP;
	int* curSP;
	int* curEXP;

	Button* btnCloss;
	Button* btnStatus;
	
	Quad* basicUI;
	Quad* hpBarBG;
	Quad* mpBarBG;

	ProgressBar* HPBar;
	ProgressBar* MPBar;
	ProgressBar* EXPBar;
	ProgressBar* SPBar;

	bool isStatusOpen = false;
};
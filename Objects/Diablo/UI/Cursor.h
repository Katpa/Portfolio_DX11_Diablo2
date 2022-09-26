#pragma once

class Cursor : public AnimObject
{
public:
	Cursor();
	~Cursor();

	void Update();
	void Render();

	void GetUIManager(UIManager* uiManager) { this->uiManager = uiManager; }
	void GetAStarPlus(AStarPlus* astarPlus) { this->aStarPlus = astarPlus; }
private:
	void SetClip();

private:
	UIManager* uiManager;
	AStarPlus* aStarPlus;

	class Playable* player;
};
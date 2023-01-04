#pragma once

class Cursor : public AnimObject
{
public:
	Cursor();
	~Cursor();

	void Update();
	void Render();

	void PickItem(Item* item) { pickingItem = item; }

	void GetUIManager(UIManager* uiManager) { this->uiManager = uiManager; }
	void GetInventory(Inventory* inventory);
	void GetCain(class Cain* cain) { this->cain = cain; }
	void GetConverter(Converter* converter) { this->converter = converter; }
	void GetPlayable(class Playable* player) { this->player = player; }
	void GetAStarPlus(AStarPlus* astarPlus) { this->aStarPlus = astarPlus; }
private:
	void Control();

	void SetClip();

private:
	Vector2 mouseAxis;

	Item* pickingItem;
	Item* overItem;

	UIManager* uiManager;
	AStarPlus* aStarPlus;
	Inventory* inventory;
	Cain* cain;
	Converter* converter;
	class Playable* player;
};
#pragma once

class Converter : public Quad
{
private:
	const float HALF_SLOT_SIZE = SLOT_SIZE * 0.5f;

public:
	Converter();
	~Converter();

	void Update();
	void Render();

	void Picking(Item* item);
	bool IsAblePlaceItem(Item* item, Vector2& startPos);
	void PlaceItem(Vector2 startPos, Item* item);

	bool IsMouseOn() { return isMouseOn; }
	bool& IsPicked() { return isPicked; }
	Item*& ReturnOverItem() { return overItem; }

private:
	void InitSlots();
	void InitList();

	void Convert();
	bool CheckItems();

	bool CheckList(string key, vector<int>& index);

	void GetItem(Item* item);
	bool FindSpace(Vector2 itemSize, Vector2& itemPos);
	bool IsPlace(Vector2 startPos, Vector2 size);

	Item* IsOverItem();
	void ClearItemPos(Item* item);

	void Close();

private:
	vector<Item*> items;
	ItemSlot slots[4][3];

	Effect* effect;

	UpDownButton* convertButton;
	UpDownButton* closeButton;

	vector<vector<string>> convertList;
	string convertItemKey;

	Item* overItem = nullptr;

	bool isMouseOn = false;
	bool isPicked = false;

	RectCollider* collider;
};
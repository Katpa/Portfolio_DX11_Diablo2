#pragma once

class Cain : public AnimObject
{
private:
	const float HALF_SLOT_SIZE = 14.5f;

public:
	Cain();
	~Cain();

	void Update();
	void Render() override;
	void RenderUI();

	void OpenStore();
	void CloseStore();
	void SwitchStore();

	Item* BuyItem();
	int SellItem(Item* item);

	bool IsMouseOn() { return isMouseOn; }
	bool IsMouseOnCain() { return isMouseOnCain; }
	RectCollider* ReturnCollider() { return collider; }
	Item*& ReturnOverItem() { return overItem; }

private:
	void SetSlots();

	void GetItem(Item* item);
	void PlaceItem(Vector2 startPos, Item* item);
	bool FindSpace(Vector2 itemSize, Vector2& itemPos);
	bool IsPlace(Vector2 startPos, Vector2 size);

	Item* IsOverItem();
	void ClearItemPos(Item* item);

	void InitItem();

private:
	RectCollider* collider;
	ItemSlot slots[10][10];
	vector<Item*> items;

	Quad* storeBoard;
	RectCollider* storeCollider;
	UpDownButton* closeButton;

	FloatValueBuffer* floatValueBuffer;

	Item* overItem = nullptr;
	bool isMouseOnCain = false;
	bool isMouseOn = false;
};
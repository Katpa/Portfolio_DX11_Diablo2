#pragma once

class Item : public Quad
{
public:
	enum class ItemState
	{
		NONE, OVER, PICK, COLLIDE
	};

	enum ItemType
	{
		CONSUMABLE = 0,
		HANDL = 1,
		HANDR = 2,
		ARMOR = 3,
		BELT = 4,
		BOOTS = 5,
		GLOVE = 6,
		HELM = 7
	};

	enum ItemEffect
	{
		DMG, DEF, HP, MP, HMP
	};

private:
	struct Colors
	{
		Float4 none = { 1.0f, 1.0f, 1.0f, 1.0f };
		Float4 green = { 0.3f, 1.0f, 0.3f, 1.0f };
		Float4 red = { 1.0f, 0.3f, 0.3f, 1.0f };
		Float4 picked = { 1.0f, 1.0f, 1.0f, 0.8f };
	};

public:
	Item(string key, wstring file, wstring info, ItemType type, Vector2 size, ItemEffect effect);
	~Item();

	virtual void Update() = 0;
	virtual void Render();
	void InfoRender();

	virtual POINT Use() = 0;						//1회성 효과를 줘야 하고
	virtual POINT Equip() = 0;						//지속 효과를 줘야 하고

	void Picked();

	void ShowInfo();
	void HideInfo();

	bool IsCollision(Vector2 point) { return collider->IsPointCollision(point); }

	POINT ReturnEffectPower() { return effectPower; }
	ItemType& ReturnType() { return type; }
	vector<Vector2>& ReturnInventoryPos() { return inventoryPos; }
	Vector2 ReturnSize() { return size; }
	int ReturnCost() { return cost; }
	string ReturnKey() { return key; }

protected:
	RectCollider* collider;
	Quad* info;

	string key;
	ItemEffect effect;
	POINT effectPower;

	bool isCollideItem = false;

	ItemState state = ItemState::NONE;
	ItemType type;
	Colors colors;

	int cost;
	vector<Vector2> inventoryPos;
	Vector2 size;
};
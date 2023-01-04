#pragma once

class Consumable : public Item
{
public:
	Consumable(ItemData data);
	~Consumable();

	virtual void Update() override;
	virtual POINT Use() override;
	virtual POINT Equip() override;
};
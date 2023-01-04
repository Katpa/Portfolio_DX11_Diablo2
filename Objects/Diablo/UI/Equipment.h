#pragma once

class Equipment : public Item
{
public:
	Equipment(ItemData data);
	~Equipment();

	virtual void Update() override;
	virtual POINT Use() override;
	virtual POINT Equip() override;
};
#include "Framework.h"

Consumable::Consumable(ItemData data)
    :Item(data.key, data.file, data.info, static_cast<ItemType>(data.type), data.size, static_cast<ItemEffect>(data.itemEffect))
{
	cost = data.cost;

	switch (effect)
	{
	case Item::HP:
		effectPower = { data.effectPower, 0 };
		break;
	case Item::MP:
		effectPower = { 0, data.effectPower };
		break;
	case Item::HMP:
		effectPower = { data.effectPower, data.effectPower };
		break;
	default:
		break;
	}
}

Consumable::~Consumable()
{
}

void Consumable::Update()
{
	Item::Update();
}

POINT Consumable::Use()
{
	isActive = false;

    return effectPower;
}

POINT Consumable::Equip()
{
    return POINT();
}

#include "Framework.h"

Equipment::Equipment(ItemData data)
	:Item(data.key, data.file, data.info, static_cast<ItemType>(data.type), data.size, static_cast<ItemEffect>(data.itemEffect))
{
	cost = data.cost;

	switch (effect)
	{
	case Item::DMG:
		effectPower = { data.effectPower, 0 };
		break;
	case Item::DEF:
		effectPower = { 0, data.effectPower };
		break;
	default:
		break;
	}
}

Equipment::~Equipment()
{
}

void Equipment::Update()
{
	Item::Update();
}

POINT Equipment::Use()
{
	return POINT();
}

POINT Equipment::Equip()
{
	return effectPower;
}

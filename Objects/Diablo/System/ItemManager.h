#pragma once

// ----What tasks should this manager do?----
//	1. Pooling items
//	2. Allocate drop item on Monster

class ItemManager : public Singleton<ItemManager>
{
public:
	ItemManager();
	~ItemManager();

	Item* GetItem(string key);

private:
	void LoadItemDatas();
	void LoadEquip(string itemName);
	void LoadConsume(string itemName);

private:
	map<string, vector<Consumable*>> consumable;
	map<string, vector<Equipment*>> equipment;
};
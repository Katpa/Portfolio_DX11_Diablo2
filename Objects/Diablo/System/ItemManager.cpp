#include "Framework.h"

ItemManager::ItemManager()
{
    LoadItemDatas();
}

ItemManager::~ItemManager()
{
    for (pair<string, vector<Equipment*>> map : equipment)
    {
        for (Equipment* equip : map.second)
        {
            delete equip;
        }
    }

    for (pair<string, vector<Consumable*>> map : consumable)
    {
        for (Consumable* consume : map.second)
        {
            delete consume;
        }
    }
}

Item* ItemManager::GetItem(string key)
{
    if (key == "hpPotion" || key == "mpPotion" || key == "hmpPotion")
    {
        for (Consumable* consume : consumable[key])
        {
            if (!consume->Active())
            {
                consume->Active() = true;
                return consume;
            }
        }
    }
    else
    {
        for (Equipment* equip : equipment[key])
        {
            if (!equip->Active())
            {
                equip->Active() = true;
                return equip;
            }
        }
    }

    return nullptr;
}

void ItemManager::LoadItemDatas()
{
    vector<string> itemNames;
    itemNames.push_back("belt");
    itemNames.push_back("boots");
    itemNames.push_back("greatSword");
    itemNames.push_back("heavyArmor");
    itemNames.push_back("helm");
    itemNames.push_back("glove");
    itemNames.push_back("lightArmor");
    itemNames.push_back("lightSaber");
    itemNames.push_back("wand");

    for(string itemName : itemNames)
        LoadEquip(itemName);

    itemNames.clear();
    itemNames.push_back("hpPotion");
    itemNames.push_back("mpPotion");
    itemNames.push_back("hmpPotion");

    for (string itemName : itemNames)
        LoadConsume(itemName);

}

void ItemManager::LoadEquip(string itemName)
{
    equipment[itemName].resize(5);
    for (Equipment*& equip : equipment[itemName])
    {
        equip = new Equipment(DataManager::Get()->GetItemData(itemName));
        equip->Active() = false;
    }
}

void ItemManager::LoadConsume(string itemName)
{
    consumable[itemName].resize(20);
    for (Consumable*& consume : consumable[itemName])
    {
        consume = new Consumable(DataManager::Get()->GetItemData(itemName));
        consume->Active() = false;
    }
}
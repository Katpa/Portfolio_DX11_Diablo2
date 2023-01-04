#pragma once

struct ItemData
{
	string key;
	wstring file;
	wstring info;
	int type;
	Vector2 size;
	int itemEffect;
	int effectPower;
	int cost;
};

struct AnimData
{
	wstring file;
	int animSize;
};

struct CharacterData
{
	string key;

	AnimData idle;
	AnimData atk1;
	AnimData atk2;
	AnimData cast;
	AnimData getHit;
	AnimData walk;
	AnimData run;
	AnimData death;

	UINT maxHP;
	UINT maxMP;
	UINT maxSP;

	UINT str;
	UINT dex;
	UINT eng;
	UINT via;
};

struct MonsterData
{
	string name;
	int type;
	AnimData idle;
	AnimData atk1;
	float atkFrame;
	AnimData cast;
	AnimData getHit;
	AnimData walk;
	AnimData death;

	int dmg;
	int def;
	float speed;
	UINT maxHP;
};

class DataManager : public Singleton<DataManager>
{
public:
	DataManager();
	~DataManager();

	CharacterData GetCharacterData(string key) { return characterDatas[key]; }
	MonsterData GetMonsterData(string key) { return monsterDatas[key]; }
	ItemData GetItemData(string key) { return itemDatas[key]; }
	int GetExpData(int level) { return levelDatas[level]; }

private:
	void LoadCharacterData();
	void LoadMonsterData();
	void LoadItemData();
	void LoadExpData();

private:
	map<string, CharacterData> characterDatas;
	map<string, MonsterData> monsterDatas;
	map<string, ItemData> itemDatas;
	map<int, int> levelDatas;
};
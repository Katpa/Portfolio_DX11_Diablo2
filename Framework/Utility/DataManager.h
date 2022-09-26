#pragma once

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

class DataManager : public Singleton<DataManager>
{
public:
	DataManager();
	~DataManager();

	CharacterData GetCharacterData(string key) { return characterDatas[key]; }

private:

	void LoadCharacterData();

private:
	map<string, CharacterData> characterDatas;
};
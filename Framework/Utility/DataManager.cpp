#include "Framework.h"
#include "DataManager.h"

DataManager::DataManager()
{
	LoadCharacterData();
    LoadMonsterData();
    LoadItemData();
    LoadExpData();
}

DataManager::~DataManager()
{
}

void DataManager::LoadCharacterData()
{
    FILE* file;
    fopen_s(&file, "DataTable/CharacterData.csv", "r");

    bool isFirstLine = true;

    while (true)
    {
        char temp[1024];
        fgets(temp, 1024, file);

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        if (feof(file))
            return;

        string str = temp;
        Replace(&str, "\n", "");

        vector<string> table = SplitString(str, ",");

        CharacterData data;

        data.key = table[0];

        data.idle.file.assign(table[1].begin(), table[1].end());
        data.idle.animSize = stoi(table[2]);

        data.atk1.file.assign(table[3].begin(), table[3].end());
        data.atk1.animSize = stoi(table[4]);

        data.atk2.file.assign(table[5].begin(), table[5].end());
        data.atk2.animSize = stoi(table[6]);

        data.cast.file.assign(table[7].begin(), table[7].end());
        data.cast.animSize = stoi(table[8]);

        data.getHit.file.assign(table[9].begin(), table[9].end());
        data.getHit.animSize = stoi(table[10]);

        data.walk.file.assign(table[11].begin(), table[11].end());
        data.walk.animSize = stoi(table[12]);

        data.run.file.assign(table[13].begin(), table[13].end());
        data.run.animSize = stoi(table[14]);

        data.death.file.assign(table[15].begin(), table[15].end());
        data.death.animSize = stoi(table[16]);

        data.maxHP = stoi(table[17]);
        data.maxMP = stoi(table[18]);
        data.maxSP = stoi(table[19]);

        data.str = stoi(table[20]);
        data.dex = stoi(table[21]);
        data.eng = stoi(table[22]);
        data.via = stoi(table[23]);

        characterDatas[data.key] = data;
    }
}

void DataManager::LoadMonsterData()
{
    FILE* file;
    fopen_s(&file, "DataTable/MonsterData.csv", "r");

    bool isFirstLine = true;

    while (true)
    {
        char temp[1024];
        fgets(temp, 1024, file);

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        if (feof(file))
            return;

        string str = temp;
        Replace(&str, "\n", "");

        vector<string> table = SplitString(str, ",");

        MonsterData data;

        data.name = table[0];
        data.type = stoi(table[1]);

        data.idle.file.assign(table[2].begin(), table[2].end());
        data.idle.animSize = stoi(table[3]);

        data.atk1.file.assign(table[4].begin(), table[4].end());
        data.atk1.animSize = stoi(table[5]);

        data.atkFrame = stof(table[6]);

        data.cast.file.assign(table[7].begin(), table[7].end());
        data.cast.animSize = stoi(table[8]);

        data.getHit.file.assign(table[9].begin(), table[9].end());
        data.getHit.animSize = stoi(table[10]);

        data.walk.file.assign(table[11].begin(), table[11].end());
        data.walk.animSize = stoi(table[12]);

        data.death.file.assign(table[13].begin(), table[13].end());
        data.death.animSize = stoi(table[14]);

        data.dmg = stoi(table[15]);
        data.def = stoi(table[16]);
        data.speed = stof(table[17]);
        data.maxHP = stoi(table[18]);

        monsterDatas[data.name] = data;
    }
}

void DataManager::LoadItemData()
{
    FILE* file;
    fopen_s(&file, "DataTable/ItemData.csv", "r");

    bool isFirstLine = true;

    while (true)
    {
        char temp[1024];
        fgets(temp, 1024, file);

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        if (feof(file))
            return;

        string str = temp;
        Replace(&str, "\n", "");

        vector<string> table = SplitString(str, ",");

        ItemData data;

        data.key = table[0];

        data.file.assign(table[1].begin(), table[1].end());
        data.info.assign(table[2].begin(), table[2].end());

        data.type = stoi(table[3]);
        data.size.x = stoi(table[4]);
        data.size.y = stoi(table[5]);

        data.itemEffect = stoi(table[6]);
        data.effectPower = stoi(table[7]);
        data.cost = stoi(table[8]);

        itemDatas[data.key] = data;
    }
}

void DataManager::LoadExpData()
{
    FILE* file;
    fopen_s(&file, "DataTable/LevelData.csv", "r");

    bool isFirstLine = true;

    while (true)
    {
        char temp[1024];
        fgets(temp, 1024, file);

        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        if (feof(file))
            return;

        string str = temp;
        Replace(&str, "\n", "");

        vector<string> table = SplitString(str, ",");

        int level = stoi(table[0]);
        int exp = stoi(table[1]);

        levelDatas[level] = exp;
    }
}

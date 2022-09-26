#include "Framework.h"

DataManager::DataManager()
{
	LoadCharacterData();
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
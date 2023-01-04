#include "Framework.h"

MonsterManager::MonsterManager()
{
    InitMonsters();
}

MonsterManager::~MonsterManager()
{
    for (pair<string, vector<Monster*>> vec : monsters)
    {
        for (Monster* monster : vec.second)
        {
            delete monster;
        }
    }
}

void MonsterManager::Update()
{
    //overMonster = nullptr;
    int aliveCount = 0;

    for (pair<string, vector<Monster*>> vec : monsters)
    {
        for (Monster* monster : vec.second)
        {
            monster->Update();
            if (monster->Active())
                aliveCount++;
        }
    }

    if (!isReviveDiablo && aliveCount == 0)
        SpawnBoss();
}

void MonsterManager::Render()
{
}

void MonsterManager::RenderUI()
{
    if (overMonster)
        overMonster->RenderUI();
}

Monster* MonsterManager::IsCollision(Vector2 mouseAxis)
{
    Monster* output = nullptr;

    for (pair<string, vector<Monster*>> vec : monsters)
    {
        for (Monster* monster : vec.second)
        {
            if (!monster->Active()) continue;

            monster->IsOver() = false;

            if (monster->ReturnBody()->IsPointCollision(mouseAxis) && !monster->IsDead())
            {
                output = monster;
            }
        }
    }

    if (output)
        output->IsOver() = true;

    overMonster = output;

    return output;
}

Monster* MonsterManager::IsCollision(Collider* collider)
{
    for (pair<string, vector<Monster*>> vec : monsters)
    {
        for (Monster* monster : vec.second)
        {
            if (!monster->Active()) continue;

            if (monster->ReturnBody()->IsCollision(collider))
            {
                return monster;
            }
        }
    }

    return nullptr;
}

void MonsterManager::InitMonsters()
{
    monsters["BlunderBore"].resize(30);

    for (Monster*& monster : monsters["BlunderBore"])
    {
        monster = new Monster("BlunderBore");
    }

    monsters["CouncilMember"].resize(30);

    for (Monster*& monster : monsters["CouncilMember"])
    {
        monster = new Monster("CouncilMember");
    }

    monsters["Diablo"].resize(30);

    for (Monster*& monster : monsters["Diablo"])
    {
        monster = new Diablo();
    }
}

void MonsterManager::StartMonsterSpawn()
{
    SpawnMonster("BlunderBore", Vector2(800, 2150));
    SpawnMonster("BlunderBore", Vector2(850, 2110));
    SpawnMonster("BlunderBore", Vector2(770, 2200));
    SpawnMonster("BlunderBore", Vector2(800, 2250));
    SpawnMonster("BlunderBore", Vector2(750, 2300));
    SpawnMonster("CouncilMember", Vector2(825, 2300));
    SpawnMonster("CouncilMember", Vector2(775, 2050));
    SpawnMonster("CouncilMember", Vector2(720, 2300));
    
    SpawnMonster("CouncilMember", Vector2(2720, 1200));
    SpawnMonster("CouncilMember", Vector2(2800, 1270));
    SpawnMonster("CouncilMember", Vector2(2660, 1300));
    SpawnMonster("CouncilMember", Vector2(2900, 1220));
    SpawnMonster("CouncilMember", Vector2(2777, 1325));
    SpawnMonster("BlunderBore", Vector2(2805, 1150));
    SpawnMonster("BlunderBore", Vector2(2900, 1300));

    SpawnMonster("CouncilMember", Vector2(3300, 2500));
    SpawnMonster("CouncilMember", Vector2(3100, 2250));
    SpawnMonster("CouncilMember", Vector2(3500, 2250));
    SpawnMonster("CouncilMember", Vector2(3350, 2400));
    SpawnMonster("CouncilMember", Vector2(3600, 2700));
    SpawnMonster("BlunderBore", Vector2(3100, 2350));
    SpawnMonster("BlunderBore", Vector2(3200, 2500));
    SpawnMonster("BlunderBore", Vector2(3000, 2600));
    SpawnMonster("BlunderBore", Vector2(3552, 2570));

    SpawnMonster("CouncilMember", Vector2(6684, 2000));
    SpawnMonster("CouncilMember", Vector2(6500, 1950));
    SpawnMonster("CouncilMember", Vector2(6300, 2100));
    SpawnMonster("CouncilMember", Vector2(6300, 2400));
    SpawnMonster("BlunderBore", Vector2(6370, 2250));
    SpawnMonster("BlunderBore", Vector2(6800, 1780));
    SpawnMonster("BlunderBore", Vector2(6500, 2500));
    SpawnMonster("BlunderBore", Vector2(6100, 2250));
    SpawnMonster("BlunderBore", Vector2(600, 1994));

    SpawnMonster("CouncilMember", Vector2(4800, 2340));
    SpawnMonster("CouncilMember", Vector2(5000, 1950));
    SpawnMonster("CouncilMember", Vector2(4950, 2100));
    SpawnMonster("CouncilMember", Vector2(4600, 2100));
    SpawnMonster("BlunderBore", Vector2(4850, 2100));
    SpawnMonster("BlunderBore", Vector2(5050, 1990));
    SpawnMonster("BlunderBore", Vector2(4700, 2200));
    SpawnMonster("BlunderBore", Vector2(4900, 2300));
    SpawnMonster("BlunderBore", Vector2(5000, 2500));
}

void MonsterManager::SpawnBoss()
{
    isReviveDiablo = true;
    //카메라 쉐이킹
    CAM->CameraShakeOn();
    SpawnMonster("Diablo", Vector2(4200, 2100));

    SFX->Stop("tristram");
    SFX->Play("mesa", 0.1f);
    SFX->Play("diabloIntro", 0.1f);
}

void MonsterManager::SetMonster(Playable* player, AStar* astar)
{
    for (pair<string, vector<Monster*>> vec : monsters)
    {
        for (Monster* monster : vec.second)
        {
            monster->GetPlayer(player);
            monster->GetAstar(astar);
        }
    }
}

void MonsterManager::SpawnMonster(string name, Vector2 pos)
{
    for (Monster* monster : monsters[name])
    {
        if (!monster->Active())
        {
            monster->Spawn(pos);
            break;
        }
    }
}

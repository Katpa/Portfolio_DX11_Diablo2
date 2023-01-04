#pragma once

class MonsterManager : public Singleton<MonsterManager>
{
public:
	MonsterManager();
	~MonsterManager();

	void Update();
	void Render();
	void RenderUI();

	void StartMonsterSpawn();
	void SpawnBoss();
	void SetMonster(Playable* player, AStar* astar);
	void SpawnMonster(string name, Vector2 pos);

	Monster* IsCollision(Vector2 mouseAxis);
	Monster* IsCollision(Collider* collider);

private:
	void InitMonsters();

private:
	map<string, vector<Monster*>> monsters;
	Monster* overMonster;

	bool isReviveDiablo = false;
};
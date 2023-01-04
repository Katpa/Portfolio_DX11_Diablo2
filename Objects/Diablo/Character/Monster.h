#pragma once

enum MonsterType
{
	MELEE = 0, RANGE = 1
};

class Monster : public Character
{
private:
	const float ANGLE_FACTOR = 0.125f;
	const float PI_ANGLE = XM_PI * ANGLE_FACTOR;

public:
	Monster() {}
	Monster(string monsterName);
	~Monster();

	virtual void Update() override;
	virtual void Render() override;
	void RenderUI();

	void Spawn(Vector2 spawnPos);

	void GetPlayer(Playable* player) { this->player = player; }
	void GetAstar(AStar* astar) { this->astar = astar; }

	bool& IsOver() { return isOver; }
	bool IsDead() { return isDead; }

protected:
	virtual void Control() override;

	void Tasking();

	void MoveTo();
	void Attack();
	void Cast();
	void Wait();

	virtual void Move() override;

	virtual void SetAngle() override;

	void SetOutline();

	void AttackCollision();

	void SetHPBar();

	void EndTask() { isTaskOver = true; }
	void SetClip(MonsterData data);

protected:
	bool isOver = false;
	bool isDead = false;
	bool isTaskOver = false;

	string name;
	MonsterType type;

	Playable* player;

	vector<CallBack> taskQueue;

	AStar* astar;

	float chaseRange = 900;
	float attackRange = 80;
	float castRange = 400;

	float atkFrame;
	float waitTime = 1;
	float deathTime = 10;

	Vector2 dest;

	map< CharAction, Vector2> animCutSize;
	CharAction state;
	IntValueBuffer* intValueBuffer;
	FloatValueBuffer* floatValueBuffer;
	
	ProgressBar* hpBar;
};
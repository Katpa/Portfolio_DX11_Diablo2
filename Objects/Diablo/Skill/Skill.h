#pragma once

class Skill : public AnimObject
{
public:
	enum SkillType
	{
		ACTIVE, PASSIVE
	};

public:
	Skill(SkillType type, string name, float dmgFactor, float coolTime, int cost);
	virtual ~Skill() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum = 0, Character* caster = nullptr);

	virtual int GiveDmg() = 0;

	SkillType ReturnType() { return type; }
	virtual BuffDesc ReturnBuff() { return BuffDesc(); };
	float ReturnCoolTime() { return maxCoolTime; }
	int ReturnCost() { return cost; }
	virtual void SetAStar(AStarPlus* astar) {}
	void SetPlayer(Character* player) { this->player = player; }

protected:
	virtual void SetClip() = 0;

protected:
	SkillType type;

	Character* player;

	string name;
	float skillFactor;
	bool isPlayerCast;

	float curCoolTime = 0;
	float maxCoolTime;

	int cost;

	int dmg = 0;
};
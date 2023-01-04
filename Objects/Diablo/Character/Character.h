#pragma once

struct BuffDesc
{
	float extraATK = 0;
	float extraATK_Speed = 0;
	float extraDEF = 0;
};

class Character :public AnimObject
{
public:
	enum CharAction
	{
		IDLE = 0,
		MELEE_ATK_A = 16,
		MELEE_ATK_B = 32,
		CAST = 48,
		CAST2 = 56,
		GET_HIT = 64,
		WALK = 80,
		RUN = 96,
		DEATH = 112
	};

	struct CharacterCollision
	{
		CircleCollider* body;
		CircleCollider* attackArea;
	};

public:
	Character();
	~Character();

	virtual void Update() = 0;
	virtual void Render() = 0;

	void SetPath(vector<Vector2> path);
	vector<Vector2>& GetPath() { return path; }

	void GetDamage(UINT dmg);
	void GiveDamage(Character* target, UINT dmg);

	virtual float GetDepth() { return collision.body->GlobalPosition().y; }
	virtual float GetPosX() { return collision.body->GlobalPosition().x; }
	
	CircleCollider* ReturnBody() { return collision.body; }

protected:
	virtual void Control() = 0;

	virtual void Move();

	virtual void SetAngle() = 0;

	void SetAction(CharAction action);
	virtual void SetClip(CharacterData data) {};

protected:
	CharacterCollision collision;

	float speed;
	int action = 3;
	vector<Vector2> path;

	float angle = 0;
	int angleNum = 0;
	Vector2 direction = {};

	int curHP;
	UINT maxHP;
	UINT dmg;
	UINT def;
};
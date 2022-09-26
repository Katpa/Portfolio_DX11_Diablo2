#pragma once

class Character :public AnimObject
{
public:
	enum CharAction
	{
		IDLE = 0,
		MELEE_ATK_A = 16,
		MELEE_ATK_B = 32,
		CAST = 48,
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
	virtual void Render(bool isDebugMode) = 0;

	void SetPath(vector<Vector2> path);
	vector<Vector2>& GetPath() { return path; }

	void GetDamage(UINT dmg);
	void GiveDamage(Character* target, UINT dmg);

protected:
	virtual void Control() = 0;

	void Move();

	virtual void SetAngle() = 0;

	void SetAction(CharAction action);
	virtual void SetClip(CharacterData data) = 0;

protected:
	CharacterCollision collision;

	float speed;
	int action;
	vector<Vector2> path;

	float angle = 0;
	int angleNum = 0;
	Vector2 direction = {};

	int curHP;
	UINT maxHP;
	UINT dmg;
	UINT def;
};
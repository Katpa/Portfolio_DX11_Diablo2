#include "Framework.h"

Character::Character()
{
}

Character::~Character()
{
	delete collision.body;
}

void Character::SetPath(vector<Vector2> path)
{
	SetAction(WALK);

	this->path = path;
}

void Character::GetDamage(UINT dmg)
{
	if (dmg < def)
		dmg = def;

	curHP -= dmg - def;

	if (curHP < 1)
	{
		curHP = 0;
	}

	SetAction(GET_HIT);
}

void Character::GiveDamage(Character* target, UINT dmg)
{
	target->GetDamage(dmg);
}

void Character::Move()
{
	if (path.empty()) return;

	Vector2 distance = path.back() - collision.body->GlobalPosition();
	direction = distance.Normalized();
	
	//SetAngle();
	SetAction(WALK);
	localPosition += direction * speed * DELTA;

	if (distance.Length() < 5.0f)
	{
		path.pop_back();
		if (path.empty())
			SetAction(IDLE);
	}
}

void Character::SetAction(CharAction action)
{
	if (this->action == action + angleNum) return;

	this->action = action + angleNum;
	curClip = clips[this->action];
	curClip->Play();
}

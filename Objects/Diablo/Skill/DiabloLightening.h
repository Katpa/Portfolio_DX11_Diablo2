#pragma once

class DiabloLightening : public Skill
{
public:
	DiabloLightening();
	~DiabloLightening();

	virtual void Update() override;
	virtual void Render() override;

	virtual void Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum = 0, Character* caster = nullptr) override;

	virtual int GiveDmg() override;

private:
	virtual void SetClip() override;
	void Move();
	void Collision();
	void EndFX();

private:
	CircleCollider* collider;
	Vector2 direction;
	float speed = 1000;
};
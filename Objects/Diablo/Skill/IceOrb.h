#pragma once

class IceOrb : public Skill
{
private:
	const float PI_ANGLE = XM_PI * 0.0625f;

public:
	IceOrb();
	~IceOrb();

	virtual void Update() override;
	virtual void Render() override;

	virtual void Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum = 0, Character* caster = nullptr) override;

	virtual int GiveDmg() override;

private:
	virtual void SetClip() override;
	void Move();
	void Fire();
	void EndFire();
	void Collision();

	void SetAngleNum();

	void EndFX();

private:
	CircleCollider* collider;
	Vector2 direction;
	float speed = 250.0f;

	float fireFrame = 0;
	const float fireInterval = 0.09f;

	float lifeTime = 0;
	const float endTime = 2.5f;

	int angleNum;
	float angle;
	const float angleInterval = XM_2PI * 1.3f / 9;
};
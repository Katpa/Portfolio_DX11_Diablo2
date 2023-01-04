#pragma once

class FireBall : public Skill
{
public:
	FireBall();
	~FireBall();

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
	float speed = 650.0f;

	float lifeTime = 0;
	const float endTime = 3.0f;
};
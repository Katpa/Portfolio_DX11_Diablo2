#pragma once

class Teleport : public Skill
{
public:
	Teleport();
	~Teleport();


	// Skill을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;

	virtual void Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum = 0, Character* caster = nullptr) override;

	virtual int GiveDmg() override;

	virtual float GetDepth() { return GlobalPosition().y - 100; }
	virtual void SetAStar(AStarPlus* astar) override { astarPlus = astar; }

private:
	virtual void SetClip() override;

	void Warp();
	void EndFX();

private:
	Character* caster;
	AStarPlus* astarPlus;

	float maxLength = 400.0f;

	Vector2 dest;
};
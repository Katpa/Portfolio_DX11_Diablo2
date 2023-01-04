#pragma once

class Fanaticism : public Skill
{
public:
	Fanaticism();
	~Fanaticism();

	virtual void Update() override;
	virtual void Render() override;

	virtual void Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum = 0, Character* caster = nullptr) override;

	virtual int GiveDmg() override;

	virtual BuffDesc ReturnBuff() override { return buff; }

private:
	virtual void SetClip() override;

private:
	Character* caster;
	BuffDesc buff;
};
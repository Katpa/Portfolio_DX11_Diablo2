#pragma once

class Playable : public Character
{
private:
	const float ANGLE_FACTOR = 0.0625f;
	const float PI_ANGLE = XM_PI * ANGLE_FACTOR;

	struct Stats
	{
		UINT strength;
		UINT dexterlity;
		UINT energy;
		UINT viality;
	};

public:
	Playable(UINT maxHP, UINT maxMP, UINT maxSP, UINT str, UINT dex, UINT eng, UINT via);
	Playable(CharacterData data);
	~Playable();

	virtual void Update() override;
	virtual void Render(bool isDebugMode) override;

protected:
	virtual void Control() override;
	
	void CursorAction();

	void SetStats();
	virtual void SetAngle() override;

	virtual void SetClip(CharacterData data) override;

private:
	//Inventory* inventory;
	//Skill* skills[3];
	UIManager* uiManager;

	const UINT defaultHP;
	const UINT defaultMP;
	const UINT defaultSP;

	int curMP;
	UINT maxMP;
	int curSP;
	UINT maxSP;

	int curEXP = 0;

	Stats stat;
};
#pragma once

struct Stats
{
	UINT strength;
	UINT dexterlity;
	UINT energy;
	UINT viality;

	int statPoint = 10;
};

class Playable : public Character
{
private:
	const float ANGLE_FACTOR = 0.0625f;
	const float PI_ANGLE = XM_PI * ANGLE_FACTOR;

	enum PlayerType
	{
		PA, SO
	};

public:
	Playable(CharacterData data);
	~Playable();

	virtual void Update() override;
	virtual void Render() override;
	void RenderUI();

	void GetExp(UINT exp);
	void GetMoney(UINT gold);
	bool SpendMoney(UINT gold);

	void Cast(Vector2 mousePoint);
	void AdaptItem(POINT effectPower);

	UIManager* ReturnUIManager() { return uiManager; }
	Inventory* ReturnInventory() { return inventory; }
	Cain*& ReturnNPC() { return npc; }
	Converter* ReturnConverter() { return converter; }

protected:
	virtual void Control() override;

	virtual void Move() override;

	void InteractNPC();

	void LevelUp();
	void Recover();

	void SetSkill(string skillName);
	void SetStats();
	virtual void SetAngle() override;

	void EndCast();

	virtual void SetClip(CharacterData data) override;

private:
	Inventory* inventory;
	UIManager* uiManager;
	Converter* converter;

	const UINT defaultHP;
	const UINT defaultMP;
	const UINT defaultSP;

	int curMP;
	UINT maxMP;
	float curSP = 0;
	UINT maxSP;
	int curEXP;
	UINT maxEXP;

	float autoHealFrame = 0;
	float autoRegenerateFrame = 0;

	Stats stat;

	UINT level = 1;
	int statPoint = 0;

	string mouseSkill;
	float skillCoolTime = 0;

	string lastBuffName;
	BuffDesc buff;

	int gold = 10000;

	Cain* npc = nullptr;

	bool isCast = false;
	bool isAttack = false;
	bool isRun = true;
};
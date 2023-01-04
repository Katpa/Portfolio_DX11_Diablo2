#pragma once

class SkillManager : public Singleton<SkillManager>
{
public:
	SkillManager();
	~SkillManager();

	void CastSkill(string skillName, bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster = nullptr);
	void Update();

	bool IsPassive(string skillName);
	Skill* ReturnBuffSkill(string skillName) { return skillData[skillName][0]; }
	int ReturnSkillCost(string skillName) { return skillData[skillName][0]->ReturnCost(); }
	float ReturnCoolTime(string skillName) { return skillData[skillName][0]->ReturnCoolTime(); }

	void GetAStarPlus(AStarPlus* astar);
	void SetPlayer(Character* player);

private:
	void SetSkills();

private:
	map<string, vector<Skill*>>skillData;
};
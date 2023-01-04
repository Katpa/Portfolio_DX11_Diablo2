#include "Framework.h"

SkillManager::SkillManager()
{
	SetSkills();
}

SkillManager::~SkillManager()
{
	for (pair < string, vector<Skill*>> vec : skillData)
	{
		for (Skill* skill : vec.second)
		{
			delete skill;
		}
	}
}

void SkillManager::CastSkill(string skillName, bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster)
{
	for (Skill* skill : skillData[skillName])
	{
		if (!skill->Active())
		{
			skill->Cast(isPlayerCast, dmg, castPos, dest, angleNum, caster);
			break;
		}
	}
}

void SkillManager::Update()
{
	for (pair < string, vector<Skill*>> vec : skillData)
	{
		for (Skill* skill : vec.second)
		{
			skill->Update();
		}
	}
}

bool SkillManager::IsPassive(string skillName)
{
	if (skillData[skillName][0]->ReturnType() == Skill::SkillType::PASSIVE)
		return true;

	return false;
}

void SkillManager::GetAStarPlus(AStarPlus* astar)
{
	for (Skill*& skill : skillData["teleport"])
		skill->SetAStar(astar);
}

void SkillManager::SetPlayer(Character* player)
{
	for (pair < string, vector<Skill*>> vec : skillData)
	{
		for (Skill* skill : vec.second)
		{
			skill->SetPlayer(player);
		}
	}
}

void SkillManager::SetSkills()
{
	//���̽���Ʈ
	skillData["iceBolt"].resize(200);
	for (Skill*& skill : skillData["iceBolt"])
		skill = new IceBolt();

	//���̽�����
	skillData["iceOrb"].resize(10);
	for (Skill*& skill : skillData["iceOrb"])
		skill = new IceOrb();

	//���̾
	skillData["fireBall"].resize(20);
	for (Skill*& skill : skillData["fireBall"])
		skill = new FireBall();

	//�ڷ���Ʈ
	skillData["teleport"].resize(5);
	for (Skill*& skill : skillData["teleport"])
		skill = new Teleport();

	//�ĳ�ƽ����
	skillData["fanaticism"].resize(1);
	for (Skill*& skill : skillData["fanaticism"])
		skill = new Fanaticism();

	//��ƺ�� ����Ʈ��
	skillData["diabloLightening"].resize(50);
	for (Skill*& skill : skillData["diabloLightening"])
		skill = new DiabloLightening();

	//��ƺ�� �Ҳ� ���ڱ�
	skillData["fireStomp"].resize(150);
	for (Skill*& skill : skillData["fireStomp"])
		skill = new FireStomp();
}
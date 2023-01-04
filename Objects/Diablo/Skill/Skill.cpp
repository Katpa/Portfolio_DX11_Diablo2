#include "Framework.h"

Skill::Skill(SkillType type, string name, float dmgFactor, float coolTime, int cost)
	:type(type), name(name), skillFactor(dmgFactor), maxCoolTime(coolTime), cost(cost)
{
}

void Skill::Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster)
{
}
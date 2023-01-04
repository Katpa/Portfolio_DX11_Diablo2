#include "Framework.h"

Playable::Playable(CharacterData data)
	:Character(), defaultHP(data.maxHP), defaultMP(data.maxMP), defaultSP(data.maxSP),
				  maxMP(data.maxMP), curMP(data.maxMP), maxSP(data.maxSP), curSP(data.maxSP)
{
	stat.strength = data.str;
	stat.dexterlity = data.dex;
	stat.energy = data.eng;
	stat.viality = data.via;

	speed = 200.0f;

	SetClip(data);

	collision.body = new CircleCollider(20.0f);
	collision.body->SetParent(this);
	collision.body->UpdateWorld();

	uiManager = new UIManager(&stat.strength, &stat.dexterlity, &stat.energy, &stat.viality,
							&curHP, &curMP, &curSP, &curEXP, &maxHP, &maxMP, &maxSP, &maxEXP, &level,
							&isRun, &stat.statPoint, &dmg, &def);

	inventory = new Inventory(&gold);
	inventory->Active() = false;

	converter = new Converter();

	maxHP = data.maxHP;

	SetAction(IDLE);
	Scale() *= TEX_SCALE_MULTI;

	maxEXP = DataManager::Get()->GetExpData(level);

	ZOrder::Get()->Insert(this);

	mouseSkill = "fireBall";
	lastBuffName = "none";

	SetStats();
	curHP = maxHP;
}

Playable::~Playable()
{
	delete uiManager;
	delete inventory;
	delete converter;
}

void Playable::Update()
{
	SetStats();

	SetAngle();

	Control();

	Move();

	InteractNPC();

	AnimObject::Update();
	collision.body->UpdateWorld();

	uiManager->Update();
	inventory->Update();
	converter->Update();

	Recover();
}

void Playable::Render()
{
	AnimObject::Render();
}

void Playable::RenderUI()
{
	inventory->Render();
	uiManager->Render();
	converter->Render();
}

void Playable::GetExp(UINT exp)
{
	curEXP += exp;
	if (curEXP >= maxEXP)
		LevelUp();
}

void Playable::GetMoney(UINT gold)
{
	this->gold += gold;
}

bool Playable::SpendMoney(UINT gold)
{
	if (this->gold > gold)
	{
		this->gold -= gold;
		return true;
	}

	return false;
}

void Playable::Cast(Vector2 mousePoint)
{
	if (SKILL->IsPassive(mouseSkill)) return;

	if (skillCoolTime < 0.01f && SKILL->ReturnSkillCost(mouseSkill) < curMP)
	{
		direction = CAM->ScreenToWorld(mousePos) - collision.body->GlobalPosition();
		SetAngle();
		SetAction(CAST);

		curMP -= SKILL->ReturnSkillCost(mouseSkill);

		SKILL->CastSkill(mouseSkill, true, dmg, GlobalPosition(), mousePoint, angleNum, this);
		uiManager->SwitchCoolTime();
		skillCoolTime = SKILL->ReturnCoolTime(mouseSkill);

		path.clear();
	}
	else
	{
		if(!SFX->IsPlaySound("cantUseYet"))
			SFX->Play("cantUseYet", 0.1f);
	}
}

void Playable::AdaptItem(POINT effectPower)
{
	curHP += effectPower.x;
	curMP += effectPower.y;

	if (curHP > maxHP)
		curHP = maxHP;
	if (curMP > maxMP)
		curMP = maxMP;

	SFX->Play("potion", 0.1f);
}

void Playable::Control()
{
	if (KEY_DOWN('I'))
		inventory->Active() = !inventory->Active();

	if (KEY_DOWN('C'))
		uiManager->SwitchStatus();

	if (KEY_DOWN('F'))
		converter->Active() = !converter->Active();

	if (KEY_DOWN(VK_F1))
		SetSkill("fireBall");

	if (KEY_DOWN(VK_F2))
		SetSkill("teleport");
	
	if (KEY_DOWN(VK_F3))
		SetSkill("iceOrb");

	if (KEY_DOWN(VK_F4))
	{
		SetSkill("fanaticism");

		SKILL->CastSkill(mouseSkill, true, 0, GlobalPosition(), Vector2(), 0, this);
	}

	if (KEY_DOWN('R'))
	{
		isRun = !isRun;
	}
}

void Playable::Move()
{
	if (path.empty()) return;

	Vector2 distance = path.back() - collision.body->GlobalPosition();
	direction = distance.Normalized();

	if (isRun)
	{
		SetAction(RUN);
		localPosition += direction * speed * 1.8f * DELTA;
		curSP -= DELTA;
		if (curSP < 0.1f)
			isRun = false;
	}
	else
	{
		SetAction(WALK);
		localPosition += direction * speed * DELTA;
	}

	if (distance.Length() < 5.0f)
	{
		path.pop_back();
		if (path.empty())
			SetAction(IDLE);
	}
}

void Playable::InteractNPC()
{
	if (npc == nullptr) return;

	Vector2 distance = npc->Position() - Position();
	float length = distance.Length();

	if (distance.Length() < 60)
	{
		npc->OpenStore();
		inventory->Active() = true;
		npc = nullptr;
	}
}

void Playable::LevelUp()
{
	curEXP -= maxEXP;
	level++;
	maxEXP = DataManager::Get()->GetExpData(level);
	stat.statPoint += 5;

	curHP = maxHP;
	curMP = maxMP;
	curSP = maxSP;

	SFX->Play("levelup", 0.1f);
}

void Playable::Recover()
{
	autoHealFrame += stat.strength * 0.1f * DELTA;
	autoRegenerateFrame += stat.energy * 0.1f * DELTA;

	if (autoHealFrame > 1.0f)
	{
		curHP++;
		autoHealFrame -= 1.0f;
		if (curHP > maxHP)
			curHP = maxHP;
	}

	if (autoRegenerateFrame > 1.0f)
	{
		curMP++;
		autoRegenerateFrame -= 1.0f;
		if (curMP > maxMP)
			curMP = maxMP;
	}

	if(!isRun && !path.empty() || path.empty())
		curSP += DELTA;
	if (curSP > maxSP)
		curSP = maxSP;

	if (skillCoolTime > 0.01f)
	{
		skillCoolTime -= DELTA;

		if(skillCoolTime < 0.01f)
			uiManager->SwitchCoolTime();
	}
}

void Playable::SetSkill(string skillName)
{
	uiManager->SetSkillIcon(skillName);
	mouseSkill = skillName;
}

void Playable::SetStats()
{
	//착용된 장비 버프 정보
	POINT EquipEffect = inventory->ReturnEquipEffect();

	dmg = stat.strength * 2 + EquipEffect.x;
	maxHP = defaultHP + stat.viality * 5;
	maxSP = defaultSP + stat.viality * 2;
	maxMP = defaultMP + stat.energy * 5;
	def = stat.dexterlity * 2 + EquipEffect.y;

	buff = { 0, 0, 0 };

	//패시브 스킬 유무 확인 후 효과 적용
	if (SKILL->IsPassive(mouseSkill))
	{
		lastBuffName = mouseSkill;
		buff = SKILL->ReturnBuffSkill(mouseSkill)->ReturnBuff();

		dmg *= 1 + buff.extraATK;
		def *= 1 + buff.extraDEF;
	}
	else if (lastBuffName != "none")
	{
		SKILL->ReturnBuffSkill(lastBuffName)->Active() = false;
	}
}

void Playable::SetAngle()
{
	angle = direction.Angle();

	float temp = angle / PI_ANGLE;

	if (temp > 15)
		angleNum = 4;
	else if (temp > 13)
		angleNum = 5;
	else if (temp > 11)
		angleNum = 6;
	else if (temp > 9)
		angleNum = 7;
	else if (temp > 7)
		angleNum = 8;
	else if (temp > 5)
		angleNum = 9;
	else if (temp > 3)
		angleNum = 10;
	else if (temp > 1)
		angleNum = 11;
	else if (temp > -1)
		angleNum = 12;
	else if (temp > -3)
		angleNum = 13;
	else if (temp > -5)
		angleNum = 14;
	else if (temp > -7)
		angleNum = 15;
	else if (temp > -9)
		angleNum = 0;
	else if (temp > -11)
		angleNum = 1;
	else if (temp > -13)
		angleNum = 2;
	else
		angleNum = 3;
}

void Playable::EndCast()
{
	isCast = false;
	SetAction(IDLE);
}

void Playable::SetClip(CharacterData data)
{
	wstring file;
	Texture* texture;
	Vector2 cutSize;
	vector<Frame*> frames;

	file = data.idle.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.idle.animSize, 16);
	for (int i = 0; i < 16; i++)
	{
		frames.clear();
		for (int j = 0; j < data.idle.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		clips[IDLE + i] = new Clip(frames);
	}

	file = data.atk1.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.atk1.animSize, 16);
	for (int i = 0; i < 16; i++)
	{
		frames.clear();
		for (int j = 0; j < data.atk1.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		clips[MELEE_ATK_A + i] = new Clip(frames);
	}

	file = data.atk2.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.atk2.animSize, 16);
	for (int i = 0; i < 16; i++)
	{
		frames.clear();
		for (int j = 0; j < data.atk2.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		clips[MELEE_ATK_B + i] = new Clip(frames);
	}

	file = data.cast.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.cast.animSize, 16);
	for (int i = 0; i < 16; i++)
	{
		frames.clear();
		for (int j = 0; j < data.cast.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		clips[CAST + i] = new Clip(frames, Clip::END, 0.05f);
		clips[CAST + i]->SetEvent(1.0f, bind(&Playable::EndCast, this));
	}

	file = data.getHit.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.getHit.animSize, 16);
	for (int i = 0; i < 16; i++)
	{
		frames.clear();
		for (int j = 0; j < data.getHit.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		clips[GET_HIT + i] = new Clip(frames, Clip::END);
	}

	file = data.walk.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.walk.animSize, 16);
	for (int i = 0; i < 16; i++)
	{
		frames.clear();
		for (int j = 0; j < data.walk.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		clips[WALK + i] = new Clip(frames);
	}

	file = data.run.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.run.animSize, 16);
	for (int i = 0; i < 16; i++)
	{
		frames.clear();
		for (int j = 0; j < data.run.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		clips[RUN + i] = new Clip(frames);
	}

	file = data.death.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.death.animSize, 16);
	for (int i = 0; i < 16; i++)
	{
		frames.clear();
		for (int j = 0; j < data.death.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		clips[DEATH + i] = new Clip(frames, Clip::END);
	}
}
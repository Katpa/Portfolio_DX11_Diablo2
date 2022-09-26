#include "Framework.h"

Playable::Playable(UINT maxHP, UINT maxMP, UINT maxSP, UINT str, UINT dex, UINT eng, UINT via)
	:Character(), defaultHP(maxHP),defaultMP(maxMP),defaultSP(maxSP),
					maxMP(maxMP), curMP(maxMP), maxSP(maxSP), curSP(maxSP)
{
	this->maxHP = maxHP;
	curHP = maxHP;

	stat.strength = str;
	stat.dexterlity = dex;
	stat.energy = eng;
	stat.viality = via;

	speed = 300.0f;
	SetStats();
}

Playable::Playable(CharacterData data)
	:Character(), defaultHP(data.maxHP), defaultMP(data.maxMP), defaultSP(data.maxSP),
				  maxMP(data.maxMP), curMP(data.maxMP), maxSP(data.maxSP), curSP(data.maxSP)
{
	maxHP = data.maxHP;
	curHP = maxHP;

	stat.strength = data.str;
	stat.dexterlity = data.dex;
	stat.energy = data.eng;
	stat.viality = data.via;

	speed = 300.0f;
	SetStats();

	SetClip(data);

	uiManager = new UIManager(&stat.strength, &stat.dexterlity, &stat.energy, &stat.viality,
							&curHP, &curMP, &curSP, &curEXP);
}

Playable::~Playable()
{
	delete uiManager;
}

void Playable::Update()
{
	AnimObject::Update();
	collision.body->UpdateWorld();
}

void Playable::Render(bool isDebugMode)
{
	AnimObject::Render();
	if (isDebugMode)
		collision.body->Render();
}

void Playable::Control()
{
}

void Playable::CursorAction()
{

}

void Playable::SetStats()
{
	dmg = stat.strength * 2;
	maxHP = defaultHP + stat.viality * 5;
	maxSP = defaultSP + stat.viality * 2;
	maxMP = defaultMP + stat.energy * 2;
	def = stat.dexterlity * 2;
}

void Playable::SetAngle()
{
	angle = direction.Angle();

	float temp = angle / PI_ANGLE;

	if (temp > 15)
		angleNum = 12;
	else if (temp > 13)
		angleNum = 13;
	else if (temp > 11)
		angleNum = 14;
	else if (temp > 9)
		angleNum = 15;
	else if (temp > 7)
		angleNum = 0;
	else if (temp > 5)
		angleNum = 1;
	else if (temp > 3)
		angleNum = 2;
	else if (temp > 1)
		angleNum = 3;
	else if (temp > -1)
		angleNum = 4;
	else if (temp > -3)
		angleNum = 5;
	else if (temp > -5)
		angleNum = 6;
	else if (temp > -7)
		angleNum = 7;
	else if (temp > -9)
		angleNum = 8;
	else if (temp > -11)
		angleNum = 9;
	else if (temp > -13)
		angleNum = 10;
	else
		angleNum = 11;
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
		clips[CAST + i] = new Clip(frames);
	}

	file = data.getHit.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.getHit.animSize, 16);
	for (int i = 0; i < 16; i++)
	{
		frames.clear();
		for (int j = 0; j < data.getHit.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		clips[GET_HIT + i] = new Clip(frames);
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
		clips[DEATH + i] = new Clip(frames);
	}
}
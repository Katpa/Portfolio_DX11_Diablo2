#include "Framework.h"

Diablo::Diablo()
{
	isActive = false;

	Scale() *= TEX_SCALE_MULTI;

	name = "Diablo";
	maxHP = 500;
	curHP = maxHP;
	dmg = 25;
	def = 10;
	speed = 150;

	chaseRange = 1000;
	attackRange = 80;
	castRange = 400;

	waitTime = 1;
	deathTime = 30;

	SetClip();

	hpBar = new ProgressBar(L"Textures/UI/monsterHP.png", L"Textures/UI/monsterHP_bg.png", maxHP);
	hpBar->Position() = { CENTER_X, WIN_HEIGHT - 45 };

	collision.body = new CircleCollider(35.0f);
	collision.body->SetParent(this);
	collision.body->UpdateWorld();

	intValueBuffer = new IntValueBuffer();
	floatValueBuffer = new FloatValueBuffer();
	floatValueBuffer->data.values[0] = animCutSize[IDLE].x;
	floatValueBuffer->data.values[1] = animCutSize[IDLE].y;
	floatValueBuffer->data.values[2] = 0.1f;

	ZOrder::Get()->Insert(this);

	SetPixelShader(L"Shaders/PixelOutline.hlsl");
	GetColorBuffer()->Get() = { 1, 1, 0, 0.5f };
}

Diablo::~Diablo()
{
	delete hpBar;
	delete floatValueBuffer;
	delete intValueBuffer;
}

void Diablo::Update()
{
	if (!isActive) return;

	if (curHP < 1 && !isDead)
	{
		//µ¥¾²
		angleNum = 0;
		SetAction(DEATH);
		collision.body->Active() = false;
		path.clear();

		floatValueBuffer->data.values[2] = 0;
		hpBar->Active() = false;
	}

	if (isDead)
	{
		if (deathTime > 0)
		{
			if (DELTA < 0.002f)
				deathTime -= DELTA;
			else
				deathTime -= 0.001;

			if (deathTime < 0)
			{
				player->GetExp(300);
				isActive = false;
			}
		}
	}
	else
	{
		if (waitTime > 0)
		{
			waitTime -= DELTA;

			if (waitTime < 0)
				EndTask();
		}
		else
		{
			SetAngle();

			Control();

			Tasking();

			Move();
		}

		SetHPBar();

		if (isOver)
		{
			intValueBuffer->data.values[0] = 1;
			floatValueBuffer->data.values[2] = 0.08f;
			floatValueBuffer->data.values[0] = animCutSize[IDLE].x;
			floatValueBuffer->data.values[1] = animCutSize[IDLE].y;

			hpBar->Active() = true;
			//float hpValue = (float)curHP / (float)maxHP;
			//hpBar->SetValue(hpValue);
			hpBar->SetValue((float)curHP);
		}
		else
		{
			intValueBuffer->data.values[0] = 0;
			floatValueBuffer->data.values[2] = 0;

			hpBar->Active() = false;
		}
	}

	AnimObject::Update();
	collision.body->UpdateWorld();
	hpBar->UpdateWorld();
}

void Diablo::Render()
{
	Monster::Render();
}

void Diablo::Control()
{
	if (taskQueue.size() > 0) return;
	if (!isTaskOver) return;

	float distance = (player->GlobalPosition() - Position()).Length();

	if (distance > chaseRange)
	{
		taskQueue.push_back(bind(&Diablo::Wait, this));
		taskQueue.push_back(bind(&Diablo::MoveTo, this));
		dest = Position() + Vector2(Random(-100, 100), Random(-100, 100));
	}
	else
	{
		if (distance > castRange)
		{
			taskQueue.push_back(bind(&Diablo::MoveTo, this));
			dest = Position() + (player->GlobalPosition() - Position()).Normalized() * (distance + 60 - castRange);
		}
		else if (distance > attackRange)
		{
			bool random = Random(0, 2);

			if (random)
				taskQueue.push_back(bind(&Diablo::Cast, this));
			else
				taskQueue.push_back(bind(&Diablo::Cast2, this));
				
			taskQueue.push_back(bind(&Diablo::Wait, this));
		}
		else
		{
			taskQueue.push_back(bind(&Diablo::Attack, this));
			taskQueue.push_back(bind(&Diablo::Wait, this));
		}
	}
}

void Diablo::SetClip()
{
	wstring file;
	Texture* texture;
	Vector2 cutSize;
	vector<Frame*> frames;

	file = L"Textures/Character/diablo/idle.png";
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(12, 8);

	for (int i = 0; i < 8; i++)
	{
		frames.clear();

		for (int j = 0; j < 12; j++)
		{
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		}

		clips[IDLE + i] = new Clip(frames, Clip::LOOP, 0.05f);
	}
	animCutSize[IDLE] = cutSize;

	file = L"Textures/Character/diablo/walk.png";
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(12, 8);

	for (int i = 0; i < 8; i++)
	{
		frames.clear();

		for (int j = 0; j < 12; j++)
		{
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		}

		clips[WALK + i] = new Clip(frames, Clip::LOOP, 0.05f);
	}

	file = L"Textures/Character/diablo/atk02.png";
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(16, 8);

	for (int i = 0; i < 8; i++)
	{
		frames.clear();

		for (int j = 0; j < 16; j++)
		{
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		}

		clips[MELEE_ATK_A + i] = new Clip(frames, Clip::END, 0.05f);
		clips[MELEE_ATK_A + i]->SetEvent(1.0f, bind(&Diablo::EndTask, this));
	}

	file = L"Textures/Character/diablo/cast01.png";
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(18, 8);

	for (int i = 0; i < 8; i++)
	{
		frames.clear();

		for (int j = 0; j < 18; j++)
		{
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		}

		clips[CAST + i] = new Clip(frames, Clip::END, 0.05f);
		clips[CAST + i]->SetEvent(0.6f, bind(&Diablo::FireStomp, this));
		clips[CAST + i]->SetEvent(1.0f, bind(&Diablo::EndTask, this));
	}

	file = L"Textures/Character/diablo/cast02.png";
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(17, 8);

	for (int i = 0; i < 8; i++)
	{
		frames.clear();

		for (int j = 0; j < 17; j++)
		{
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		}

		clips[CAST2 + i] = new Clip(frames, Clip::END, 0.05f);
		clips[CAST2 + i]->SetEvent(0.5f, bind(&Diablo::LighteningSound, this));
		clips[CAST2 + i]->SetEvent(0.5294f, bind(&Diablo::FireLightening, this));
		clips[CAST2 + i]->SetEvent(0.5794f, bind(&Diablo::FireLightening, this));
		clips[CAST2 + i]->SetEvent(0.6294f, bind(&Diablo::FireLightening, this));
		clips[CAST2 + i]->SetEvent(0.6794f, bind(&Diablo::FireLightening, this));
		clips[CAST2 + i]->SetEvent(0.7294f, bind(&Diablo::FireLightening, this));
		clips[CAST2 + i]->SetEvent(0.7794f, bind(&Diablo::FireLightening, this));
		clips[CAST2 + i]->SetEvent(0.8235f, bind(&Diablo::FireLightening, this));
		clips[CAST2 + i]->SetEvent(1.0f, bind(&Diablo::EndTask, this));
	}

	file = L"Textures/Character/diablo/getHit.png";
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(6, 8);

	for (int i = 0; i < 8; i++)
	{
		frames.clear();

		for (int j = 0; j < 6; j++)
		{
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		}

		clips[GET_HIT + i] = new Clip(frames, Clip::END, 0.05f);
		clips[GET_HIT + i]->SetEvent(1.0f, bind(&Diablo::EndTask, this));
	}

	file = L"Textures/Character/diablo/death.png";
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(29, 5);

	frames.clear();
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 29; j++)
		{
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
		}
	}
	clips[DEATH] = new Clip(frames, Clip::END, 0.05f);
	clips[DEATH]->SetEvent(0.01f, bind(&Diablo::PlayDeath, this));
	clips[DEATH]->SetEvent(1.0f, bind(&Diablo::EndDeath, this));
}

void Diablo::MoveTo()
{
	int start = astar->FindCloseNode(GlobalPosition());
	int end = astar->FindCloseNode(dest);

	astar->GetPath(start, end, GetPath());
}

void Diablo::Attack()
{
	direction = player->ReturnBody()->GlobalPosition() - collision.body->GlobalPosition();
	direction = direction.Normalized();
	SetAngle();

	SetAction(MELEE_ATK_A);
}

void Diablo::Cast()
{
	direction = player->ReturnBody()->GlobalPosition() - collision.body->GlobalPosition();
	SetAngle();
	SetAction(CAST);
}

void Diablo::Cast2()
{
	direction = player->ReturnBody()->GlobalPosition() - collision.body->GlobalPosition();
	SetAngle();
	SetAction(CAST2);
}

void Diablo::Wait()
{
	waitTime = 1.5f;

	SetAction(IDLE);
}

void Diablo::FireLightening()
{
	SKILL->CastSkill("diabloLightening", false, dmg, GlobalPosition(), player->ReturnBody()->GlobalPosition(), angleNum, this);
}

void Diablo::LighteningSound()
{
	SFX->Play("diabloCastLazer", 0.1f);
}

void Diablo::FireStomp()
{
	float angle = 0;

	while (true)
	{
		Vector2 destDirction = { cos(angle), sin(angle) };

		SetAngle();

		SKILL->CastSkill("fireStomp", false, dmg, GlobalPosition(), GlobalPosition() + destDirction, 0);
		SKILL->CastSkill("fireStomp", false, dmg, GlobalPosition(), GlobalPosition() + destDirction * -1, 0);

		angle += PI_ANGLE;
		if (angle > XM_PI)
			break;
	}

	SFX->Play("diabloCastFire", 0.1f);
}

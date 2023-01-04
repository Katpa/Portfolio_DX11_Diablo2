#include "Framework.h"

Monster::Monster(string monsterName)
	:Character()
{
	isActive = false;

	MonsterData data = DataManager::Get()->GetMonsterData(monsterName);
	
	name = data.name;
	type = static_cast<MonsterType>(data.type);
	maxHP = data.maxHP;
	curHP = maxHP;
	atkFrame = data.atkFrame;
	dmg = data.dmg;
	def = data.def;
	speed = data.speed;

	SetClip(data);

	hpBar = new ProgressBar(L"Textures/UI/monsterHP.png", L"Textures/UI/monsterHP_bg.png", maxHP);
	hpBar->Position() = { CENTER_X, WIN_HEIGHT - 45 };

	collision.body = new CircleCollider(25.0f);
	collision.body->SetParent(this);
	//collision.body->Position().y -= 40;
	collision.body->UpdateWorld();

	SetAction(IDLE);
	Scale() *= TEX_SCALE_MULTI;

	intValueBuffer = new IntValueBuffer();
	floatValueBuffer = new FloatValueBuffer();
	floatValueBuffer->data.values[0] = animCutSize[IDLE].x;
	floatValueBuffer->data.values[1] = animCutSize[IDLE].y;
	floatValueBuffer->data.values[2] = 0.1f;

	ZOrder::Get()->Insert(this);

	SetPixelShader(L"Shaders/PixelOutline.hlsl");
	GetColorBuffer()->Get() = { 1, 1, 0, 0.5f};
}

Monster::~Monster()
{
	delete hpBar;
	delete floatValueBuffer;
	delete intValueBuffer;
}

void Monster::Update()
{
	if (!isActive) return;

	if (curHP < 1 && !isDead)
	{
		//����
		SetAction(DEATH);
		collision.body->Active() = false;
		isDead = true;
		path.clear();

		floatValueBuffer->data.values[2] = 0;
		hpBar->Active() = false;

		player->GetExp(70);
	}

	if (isDead)
	{
		if (deathTime > 0)
		{
			deathTime -= DELTA;

			if (deathTime < 0)
				isActive = false;
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

void Monster::Render()
{
	if (!isActive) return;

	intValueBuffer->SetPS(1);
	floatValueBuffer->SetPS(2);

	AnimObject::Render();
}

void Monster::RenderUI()
{
	if (!isOver) return;

	hpBar->Render();
	Font::Get()->RenderText(name, "status", hpBar->GlobalPosition());
}

void Monster::Spawn(Vector2 spawnPos)
{
	isActive = true;
	curHP = maxHP;
	Position() = spawnPos;

	isDead = false;
	isTaskOver = true;
	taskQueue.clear();
	path.clear();
	collision.body->Active() = true;
	deathTime = 5;

	Wait();
}

void Monster::Control()
{
	//���̾���
	//fsm
	//1. ������
	//2. �߰ݻ���
	//3. ���ݻ���
	//4. �״´�

	if (taskQueue.size() > 0) return;
	if (!isTaskOver) return;

	float distance = (player->GlobalPosition() - Position()).Length();

	if (distance > chaseRange)
	{
		//������ġ
		taskQueue.push_back(bind(&Monster::Wait, this));
		taskQueue.push_back(bind(&Monster::MoveTo, this));
		dest = Position() + Vector2(Random(-50, 50), Random(-50, 50));
	}
	else
	{
		if (type == MELEE)
		{
			if (distance > attackRange)
			{
				//ü�̽�
				taskQueue.push_back(bind(&Monster::MoveTo, this));
				dest = player->ReturnBody()->GlobalPosition();
			}
			else
			{
				//����
				taskQueue.push_back(bind(&Monster::Attack, this));
				taskQueue.push_back(bind(&Monster::Wait, this));
			}
		}
		else
		{
			if (distance > castRange)
			{
				//�߰�
				taskQueue.push_back(bind(&Monster::MoveTo, this));
				dest = Position() + (player->GlobalPosition() - Position()).Normalized() * (distance + 60 - castRange);
			}
			else if (distance > attackRange)
			{
				//��ų ����
				taskQueue.push_back(bind(&Monster::Cast, this));
				taskQueue.push_back(bind(&Monster::Wait, this));
			}
			else
			{
				//���� ����
				taskQueue.push_back(bind(&Monster::Attack, this));
				taskQueue.push_back(bind(&Monster::MoveTo, this));
				dest = Position() + (Position() - player->GlobalPosition()).Normalized() * 200;
				taskQueue.push_back(bind(&Monster::Wait, this));
			}
		}
	}
}

void Monster::Tasking()
{
	if (taskQueue.size() < 1) return;
	if (!isTaskOver) return;

	taskQueue[0]();

	taskQueue.erase(taskQueue.begin());

	isTaskOver = false;
}

void Monster::MoveTo()
{
	//��ã�� �˰���

	int start = astar->FindCloseNode(GlobalPosition());
	int end = astar->FindCloseNode(dest);
	
	astar->GetPath(start, end, GetPath());
}

void Monster::Attack()
{
	//  ����1->Ư�������ӿ� attackRange �ȿ� �÷��̾��� '�ٵ�'�� ��� �ְ�
	//  ����2->���� �ִϸ��̼� ������ ���͸� �÷��̾� �������� ȸ����Ų ��
	//  ����3->�� ���� ���� ���� �ٵ� ��ġ�κ��� ���� ���� ���̸� �����Ŀ� �� ���� �÷��̾� �ٵ� ���浹 �˻縦 �Ѵ�
	direction = player->ReturnBody()->GlobalPosition() - collision.body->GlobalPosition();
	direction = direction.Normalized();
	SetAngle();

	SetAction(MELEE_ATK_A);
}

void Monster::Cast()
{
	//���⼭�� �׳� �� �������� ���� �߻��ϸ� �� �ƴ� ������?
	//�׷� ������ ��ƺ�δ� Ŭ���� ���� ����Ű�
	//�׷��� �׳� ���̾ �ϳ�?
	direction = player->ReturnBody()->GlobalPosition() - collision.body->GlobalPosition();
	SetAngle();
	SetAction(CAST);

	SKILL->CastSkill("fireBall", false, dmg, GlobalPosition(), player->ReturnBody()->GlobalPosition(), angleNum * 2, this);
}

void Monster::Wait()
{
	//������Ʈ���� ���� ���־�� �Ѵ�

	waitTime = 2.0f;

	SetAction(IDLE);
}

void Monster::Move()
{
	if (path.empty()) return;

	Vector2 distance = path.back() - collision.body->GlobalPosition();
	direction = distance.Normalized();

	SetAction(WALK);
	localPosition += direction * speed * DELTA;

	float length = distance.Length();
	float tmp = (player->ReturnBody()->GlobalPosition() - collision.body->GlobalPosition()).Length();

	if (distance.Length() < 10.0f || (player->ReturnBody()->GlobalPosition() - collision.body->GlobalPosition()).Length() < 40.0f)
	{
		path.pop_back();
		if (path.empty())
		{
			SetAction(IDLE);
			EndTask();
		}
	}
}

void Monster::SetAngle()
{
	angle = direction.Angle();

	float temp = angle / PI_ANGLE;

	if (temp > 7)
		angleNum = 2;
	else if (temp > 5)
		angleNum = 3;
	else if (temp > 3)
		angleNum = 4;
	else if (temp > 1)
		angleNum = 5;
	else if (temp > -1)
		angleNum = 6;
	else if (temp > -3)
		angleNum = 7;
	else if (temp > -5)
		angleNum = 0;
	else
		angleNum = 1;
}

void Monster::SetOutline()
{
	if (isOver)
	{
		floatValueBuffer->data.values[0] = animCutSize[state].x;
		floatValueBuffer->data.values[1] = animCutSize[state].y;
		intValueBuffer->data.values[0] = 2;
	}
	else
	{
		intValueBuffer->data.values[0] = 0;
	}
}

void Monster::AttackCollision()
{
	Vector2 distance = player->ReturnBody()->GlobalPosition() - collision.body->GlobalPosition();

	if (distance.Length() < attackRange + player->ReturnBody()->Radius())
	{
		player->GetDamage(dmg);
	}
}

void Monster::SetHPBar()
{
	if (!isOver) return;

	float tmpValue = (float)curHP / (float)maxHP;
	hpBar->SetValue(tmpValue);
}

void Monster::SetClip(MonsterData data)
{
	wstring file;
	Texture* texture;
	Vector2 cutSize;
	vector<Frame*> frames;

	file = data.idle.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.idle.animSize, 8);
	for (int i = 0; i < 8; i++)
	{
		frames.clear();
		for (int j = 0; j < data.idle.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));

		clips[IDLE + i] = new Clip(frames);
	}
	animCutSize[IDLE] = cutSize;

	file = data.atk1.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.atk1.animSize, 8);
	for (int i = 0; i < 8; i++)
	{
		frames.clear();
		for (int j = 0; j < data.atk1.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));

		clips[MELEE_ATK_A + i] = new Clip(frames);
		clips[MELEE_ATK_A + i]->SetEvent(atkFrame, bind(&Monster::AttackCollision, this));
		clips[MELEE_ATK_A + i]->SetEvent(1.0f, bind(&Monster::EndTask, this));
	}
	animCutSize[MELEE_ATK_A] = cutSize;

	if (data.cast.file != L"-")
	{
		file = data.cast.file;
		texture = Texture::Add(file);
		cutSize = texture->GetSize() / Vector2(data.cast.animSize, 8);
		for (int i = 0; i < 8; i++)
		{
			frames.clear();
			for (int j = 0; j < data.cast.animSize; j++)
				frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));

			clips[CAST + i] = new Clip(frames, Clip::END, 0.05f);
			clips[CAST + i]->SetEvent(1.0f, bind(&Monster::EndTask, this));
		}
		animCutSize[CAST] = cutSize;
	}

	file = data.getHit.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.getHit.animSize, 8);
	for (int i = 0; i < 8; i++)
	{
		frames.clear();
		for (int j = 0; j < data.getHit.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));

		clips[GET_HIT + i] = new Clip(frames, Clip::END);
	}
	animCutSize[GET_HIT] = cutSize;

	file = data.walk.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.walk.animSize, 8);
	for (int i = 0; i < 8; i++)
	{
		frames.clear();
		for (int j = 0; j < data.walk.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));

		clips[WALK + i] = new Clip(frames);
	}
	animCutSize[WALK] = cutSize;

	file = data.death.file;
	texture = Texture::Add(file);
	cutSize = texture->GetSize() / Vector2(data.death.animSize, 8);
	for (int i = 0; i < 8; i++)
	{
		frames.clear();
		for (int j = 0; j < data.death.animSize; j++)
			frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));

		clips[DEATH + i] = new Clip(frames, Clip::END, 0.05f);
	}
}

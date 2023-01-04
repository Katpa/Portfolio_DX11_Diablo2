#include "Framework.h"

IceOrb::IceOrb()
    :Skill(Skill::SkillType::ACTIVE, "아이스 오브", 0, 5.0f, 20)
{
    isActive = false;
    type = ACTIVE;

    collider = new CircleCollider(10);
    collider->SetParent(this);

    SetClip();

    ZOrder::Get()->Insert(this);
}

IceOrb::~IceOrb()
{
    delete collider;
}

void IceOrb::Update()
{
    if (!isActive) return;

    AnimObject::Update();

    Move();
    Fire();
    Collision();
}

void IceOrb::Render()
{
    if (!isActive)return;

    Environment::Get()->SetAdditiveBlend();

    AnimObject::Render();

    Environment::Get()->SetAlphaBlend();
}

void IceOrb::Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster)
{
    isActive = true;

    curClip = clips[0];
    curClip->Play();
    this->isPlayerCast = isPlayerCast;

    this->dmg = dmg;

    Position() = castPos;

    direction = (dest - castPos).Normalized();

    lifeTime = 0;

    this->angleNum = 12;
    angle = 0;

    FX->Play("iceCast", castPos);
    SFX->Play("iceCast", 0.1f);
    SFX->Play("iceOrb", 0.5f);
}

int IceOrb::GiveDmg()
{
    return 0;
}

void IceOrb::SetClip()
{
    wstring file = L"Textures/Effects/IceOrb.png";
    Texture* texture = Texture::Add(file);
    Vector2 cutSize = texture->GetSize() / Vector2(16, 1);
    vector<Frame*> frames;
    
    for (int j = 0; j < 16; j++)
    {
        frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * 0, cutSize.x, cutSize.y));
    }

    clips[0] = new Clip(frames, Clip::LOOP, 0.05f);
}

void IceOrb::Move()
{
    Position() += direction * speed * DELTA;
}

void IceOrb::Fire()
{
    //angleNum 12부터 시작해서 4pi/9만큼씩 더해서 총 10번
    fireFrame += DELTA;

    if (fireFrame > fireInterval)
    {
        fireFrame -= fireInterval;

        Vector2 destDirction = { cos(angle),sin(angle) };

        SKILL->CastSkill("iceBolt", isPlayerCast, dmg, GlobalPosition(), GlobalPosition() + destDirction, angleNum);
        SKILL->CastSkill("iceBolt", isPlayerCast, dmg, GlobalPosition(), GlobalPosition() + destDirction * -1, (angleNum + 8) % 16);

        angle += angleInterval;
        if (angle > XM_PI)
            angle -= XM_2PI;

        SetAngleNum();
    }
}

void IceOrb::EndFire()
{
    angle = 0;

    while (true)
    {
        Vector2 destDirction = { cos(angle), sin(angle) };

        SetAngleNum();

        SKILL->CastSkill("iceBolt", isPlayerCast, dmg, GlobalPosition(), GlobalPosition() + destDirction, angleNum);
        SKILL->CastSkill("iceBolt", isPlayerCast, dmg, GlobalPosition(), GlobalPosition() + destDirction * -1, (angleNum + 8) % 16);

        angle += PI_ANGLE;
        if (angle > XM_PI)
            break;
    }
}

void IceOrb::Collision()
{
    //여기는 오브젝트와의 충돌만 계산하기
    //캐릭터와는 충돌 안할거임
    Character* target = MONSTER->IsCollision(collider);

    if (target != nullptr)
    {
        //맞은 대상 있다
        target->GetDamage(dmg);
        EndFX();
    }
    else
    {
        //맞은대상 없다
        lifeTime += DELTA;

        if (lifeTime > endTime)
        {
            EndFX();
        }
    }
}

void IceOrb::SetAngleNum()
{
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

void IceOrb::EndFX()
{
    isActive = false;
    EndFire();
    FX->Play("iceOrb", GlobalPosition());
}

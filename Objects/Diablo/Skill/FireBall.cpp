#include "Framework.h"

FireBall::FireBall()
    :Skill(Skill::SkillType::ACTIVE, "파이어 볼", 1.6f, 0.5f, 10)
{
    isActive = false;
    type = ACTIVE;

    collider = new CircleCollider(15);
    collider->SetParent(this);

    SetClip();

    ZOrder::Get()->Insert(this);
}

FireBall::~FireBall()
{
    delete collider;
}

void FireBall::Update()
{
    if (!isActive) return;

    //충돌처리
    Move();
    Collision();

    AnimObject::Update();
    collider->UpdateWorld();
}

void FireBall::Render()
{
    if (!isActive) return;

    Environment::Get()->SetAdditiveBlend();

    AnimObject::Render();

    Environment::Get()->SetAlphaBlend();
}

void FireBall::Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster)
{
    isActive = true;

    curClip = clips[angleNum];
    curClip->Play();
    this->isPlayerCast = isPlayerCast;

    this->dmg = (int)(dmg * skillFactor);

    Position() = castPos;

    Vector2 tmp = dest - Position();
    tmp.y *= 2;
    direction = tmp.Normalized();

    lifeTime = 0;

    UpdateWorld();
    collider->UpdateWorld();

    FX->Play("fireCast", castPos);
    SFX->Play("fireCast", 0.1f);
}

int FireBall::GiveDmg()
{
    return 0;
}

void FireBall::SetClip()
{
    wstring file = L"Textures/Effects/fireBall.png";
    Texture* texture = Texture::Add(file);
    Vector2 cutSize = texture->GetSize() / Vector2(5, 16);
    vector<Frame*> frames;

    for (int i = 0; i < 16; i++)
    {
        frames.clear();

        for (int j = 0; j < 5; j++)
        {
            frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
        }

        clips[i] = new Clip(frames, Clip::LOOP, 0.05f);
    }
}

void FireBall::Move()
{
    Vector2 tmp = direction * 0.5f;
    tmp.x *= 2;
    Position() += tmp * speed * DELTA;
}

void FireBall::Collision()
{
    Character* target = nullptr;

    if (isPlayerCast)
        target = MONSTER->IsCollision(collider);
    else if(player->ReturnBody()->IsCollision(collider))
        target = player;

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

void FireBall::EndFX()
{
    isActive = false;
    FX->Play("fireBall", GlobalPosition());
}

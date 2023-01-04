#include "Framework.h"

IceBolt::IceBolt()
    :Skill(Skill::SkillType::ACTIVE, "아이스 볼트", 1.1f, 0.5f, 8)
{
    Scale() *= TEX_SCALE_MULTI;

    isActive = false;
    type = ACTIVE;

    collider = new CircleCollider(15);
    collider->SetParent(this);

    SetClip();

    ZOrder::Get()->Insert(this);
}

IceBolt::~IceBolt()
{
    delete collider;
}

void IceBolt::Update()
{
    if (!isActive) return;

    //충돌처리
    Move();
    Collision();

    AnimObject::Update();
    collider->UpdateWorld();
}

void IceBolt::Render()
{
    if (!isActive) return;

    AnimObject::Render();
}

void IceBolt::Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster)
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

    //FX->Play("iceCast", castPos);
}

int IceBolt::GiveDmg()
{

    return 0;
}

void IceBolt::SetClip()
{
    //애니메이션 설정
    wstring file = L"Textures/Effects/iceBolt.png";
    Texture* texture = Texture::Add(file);
    Vector2 cutSize = texture->GetSize() / Vector2(6, 16);
    vector<Frame*> frames;

    for (int i = 0; i < 16; i++)
    {
        frames.clear();

        for (int j = 0; j < 6; j++)
        {
            frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
        }

        clips[i] = new Clip(frames, Clip::LOOP, 0.05f);
    }
}

void IceBolt::Move()
{
    Vector2 tmp = direction * 0.5f;
    tmp.x *= 2;
    Position() += tmp * speed * DELTA;

    //Position() += direction * 0.5f;
}

void IceBolt::Collision()
{
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

void IceBolt::EndFX()
{
    isActive = false;
    FX->Play("iceBolt", GlobalPosition());
}

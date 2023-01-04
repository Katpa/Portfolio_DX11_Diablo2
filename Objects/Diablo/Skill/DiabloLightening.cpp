#include "Framework.h"

DiabloLightening::DiabloLightening()
    :Skill(Skill::SkillType::ACTIVE, "¹ø°³¼û°á", 1, 0, 0)
{
    isActive = false;

    collider = new CircleCollider(15);
    collider->SetParent(this);

    SetClip();

    ZOrder::Get()->Insert(this);
}

DiabloLightening::~DiabloLightening()
{
    delete collider;
}

void DiabloLightening::Update()
{
    if (!isActive) return;

    Move();
    Collision();

    AnimObject::Update();
    collider->UpdateWorld();
}

void DiabloLightening::Render()
{
    if (!isActive) return;

    Environment::Get()->SetAdditiveBlend();

    AnimObject::Render();

    Environment::Get()->SetAlphaBlend();
}

void DiabloLightening::Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster)
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
    collider->Active() = true;

    UpdateWorld();
    collider->UpdateWorld();
}

int DiabloLightening::GiveDmg()
{
    return 0;
}

void DiabloLightening::SetClip()
{
    wstring file = L"Textures/Effects/diabloLightening.png";
    Texture* texture = Texture::Add(file);
    Vector2 cutSize = texture->GetSize() / Vector2(15, 8);
    vector<Frame*> frames;

    for (int i = 0; i < 8; i++)
    {
        frames.clear();

        for (int j = 0; j < 15; j++)
        {
            frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * i, cutSize.x, cutSize.y));
        }

        clips[i] = new Clip(frames, Clip::END, 0.05f);
        clips[i]->SetEvent(1.0f, bind(&DiabloLightening::EndFX, this));
    }
}

void DiabloLightening::Move()
{
    Vector2 tmp = direction * 0.5f;
    tmp.x *= 2;
    Position() += tmp * speed * DELTA;
}

void DiabloLightening::Collision()
{
    if (collider->IsCollision(player->ReturnBody()))
    {
        player->GetDamage(dmg);
        collider->Active() = false;
    }
}

void DiabloLightening::EndFX()
{
    isActive = false;
}

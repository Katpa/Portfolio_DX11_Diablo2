#include "Framework.h"

FireStomp::FireStomp()
    :Skill(Skill::SkillType::ACTIVE, "ºÒ²É ¹ßÀÚ±¹", 1, 0, 0)
{
    isActive = false;

    collider = new CircleCollider(10);
    collider->SetParent(this);

    SetClip();

    ZOrder::Get()->Insert(this);
}

FireStomp::~FireStomp()
{
    delete collider;
}

void FireStomp::Update()
{
    if (!isActive) return;

    Move();
    Collision();

    AnimObject::Update();
    collider->UpdateWorld();
}

void FireStomp::Render()
{
    if (!isActive) return;

    Environment::Get()->SetAdditiveBlend();

    AnimObject::Render();

    Environment::Get()->SetAlphaBlend();
}

void FireStomp::Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster)
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

int FireStomp::GiveDmg()
{
    return 0;
}

void FireStomp::SetClip()
{
    //37
    wstring file = L"Textures/Effects/groundFire.png";
    Texture* texture = Texture::Add(file);
    Vector2 cutSize = texture->GetSize() / Vector2(37, 1);
    vector<Frame*> frames;

    for (int i = 0; i < 37; i++)
    {
        frames.push_back(new Frame(file, cutSize.x * i, 0, cutSize.x, cutSize.y));
    }
    clips[0] = new Clip(frames, Clip::END, 0.05f);
    clips[0]->SetEvent(1.0f, bind(&FireStomp::EndFX, this));
    curClip = clips[0];
}

void FireStomp::Move()
{
    Vector2 tmp = direction * 0.5f;
    tmp.x *= 2;
    Position() += tmp * speed * DELTA;
}

void FireStomp::Collision()
{
    if (collider->IsCollision(player->ReturnBody()))
    {
        player->GetDamage(dmg);
        EndFX();
    }
}

void FireStomp::EndFX()
{
    isActive = false;

}

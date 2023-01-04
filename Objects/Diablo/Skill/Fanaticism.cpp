#include "Framework.h"

Fanaticism::Fanaticism()
    :Skill(Skill::SkillType::PASSIVE, "광신", 0, 0, 0)
{
    isActive = false;

    Scale() *= TEX_SCALE_MULTI;

    SetClip();

    buff.extraATK = 0.5f;
    buff.extraATK_Speed = 0.5f;

    ZOrder::Get()->Insert(this);
}

Fanaticism::~Fanaticism()
{
}

void Fanaticism::Update()
{
    if (!isActive) return;
    //플레이어 위치 지속적으로 받아야함
    Position() = caster->GlobalPosition() - Vector2(0, 40);

    AnimObject::Update();
}

void Fanaticism::Render()
{
    if (!isActive) return;

    Environment::Get()->SetAdditiveBlend();

    AnimObject::Render();

    Environment::Get()->SetAlphaBlend();
}

void Fanaticism::Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster)
{
    isActive = true;
    this->caster = caster;
    curClip->Play();
}

int Fanaticism::GiveDmg()
{
    return 0;
}

void Fanaticism::SetClip()
{
    wstring file = L"Textures/Effects/fanaticism.png";
    Texture* texture = Texture::Add(file);
    Vector2 cutSize = texture->GetSize() / Vector2(21, 1);
    vector<Frame*> frames;

    for (int j = 0; j < 16; j++)
    {
        frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * 0, cutSize.x, cutSize.y));
    }

    clips[0] = new Clip(frames, Clip::LOOP, 0.1f);
    curClip = clips[0];
}

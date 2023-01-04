#include "Framework.h"

Teleport::Teleport()
    :Skill(Skill::SkillType::ACTIVE, "순간이동", 0, 0.5f, 5)
{
    isActive = false;
    type = ACTIVE;

    SetClip();

    ZOrder::Get()->Insert(this);
}

Teleport::~Teleport()
{
}

void Teleport::Update()
{
    if (!isActive) return;

    AnimObject::Update();
}

void Teleport::Render()
{
    if (!isActive) return;

    Environment::Get()->SetAdditiveBlend();

    AnimObject::Render();

    Environment::Get()->SetAlphaBlend();
}

void Teleport::Cast(bool isPlayerCast, int dmg, Vector2 castPos, Vector2 dest, int angleNum, Character* caster)
{
    isActive = true;

    curClip->Play();
    this->isPlayerCast = isPlayerCast;
    this->caster = caster;

    //텔레포트 위치 잡기
    if ((dest - castPos).Length() > maxLength)
        this->dest = (dest - castPos).Normalized() * maxLength + castPos;
    else
        this->dest = dest;

    Position() = castPos;

    SFX->Play("teleport", 0.1f);
}

int Teleport::GiveDmg()
{
    return 0;
}

void Teleport::SetClip()
{
    wstring file = L"Textures/Effects/teleport.png";
    Texture* texture = Texture::Add(file);
    Vector2 cutSize = texture->GetSize() / Vector2(18, 1);
    vector<Frame*> frames;

    for (int j = 0; j < 16; j++)
    {
        frames.push_back(new Frame(file, cutSize.x * j, cutSize.y * 0, cutSize.x, cutSize.y));
    }

    clips[0] = new Clip(frames, Clip::END, 0.04f);
    clips[0]->SetEvent(0.45f, bind(&Teleport::Warp, this));
    clips[0]->SetEvent(1.0f, bind(&Teleport::EndFX, this));
    curClip = clips[0];
}

void Teleport::Warp()
{
    //A*를 이용하여 가까운 노드부터 찾기
    Vector2 warpPos = astarPlus->FindWarpPos(dest);

    caster->Position() = warpPos;
    Position() = warpPos - Vector2(-10, 0);
}

void Teleport::EndFX()
{
    isActive = false;
}

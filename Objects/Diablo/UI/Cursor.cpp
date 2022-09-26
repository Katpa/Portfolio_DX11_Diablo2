#include "Framework.h"

Cursor::Cursor()
{
	SetClip();
	ShowCursor(false);
	localScale *= 1.5f;
}

Cursor::~Cursor()
{

}

void Cursor::Update()
{
	localPosition = CAM->ScreenToWorld(mousePos) + Vector2(10, -10);

	if (KEY_DOWN(VK_LBUTTON))
	{
		//if (uiManager->IsStatusOpen())
		if(false)
		{
			//스테이터스 창 내의 충돌처리 우선적으로 처리
		}
		
		if (false)
		{
			//여기는 인벤토리
		}

		if (false)
		{
			//여기가 기본 UI
		}

		if (false)
		{
			//몬스터 충돌 검사
			if (false)
			{
				//추격 후 근접공격 시전 or 왼쪽 마우스에 할당된 스킬 시전

			}
			else
			{
				int start = aStarPlus->FindCloseNode(player->Position());
				int end = aStarPlus->FindCloseNode(mousePos);

				aStarPlus->GetPath(start, end, player->GetPath());

				player->GetPath().insert(player->GetPath().begin(), mousePos);
			}
		}
	}

	if (KEY_DOWN(VK_RBUTTON))
	{
		//소비형 아이템 위의 경우 아이템 사용
		if (false)
		{

		}

		//월드맵 위일 경우엔 거기로 오른 마우스클릭에 잡혀있는 스킬 사용
		if (false)
		{

		}
	}

	AnimObject::Update();
}

void Cursor::Render()
{
	AnimObject::Render();
}

void Cursor::SetClip()
{
	wstring file = L"Textures/UI/cursor.png";
	Texture* texture = Texture::Add(file);

	Vector2 cutSize = texture->GetSize() / Vector2(8, 1);

	vector<Frame*> frames;

	for (int i = 0; i < cutSize.x; i++)
		frames.push_back(new Frame(file, cutSize.x * i, cutSize.y * 0, cutSize.x, cutSize.y));
	
	clips[0] = new Clip(frames, Clip::LOOP, 0.25f);
	curClip = clips[0];
	curClip->Play();
}

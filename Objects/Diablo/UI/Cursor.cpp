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
			//�������ͽ� â ���� �浹ó�� �켱������ ó��
		}
		
		if (false)
		{
			//����� �κ��丮
		}

		if (false)
		{
			//���Ⱑ �⺻ UI
		}

		if (false)
		{
			//���� �浹 �˻�
			if (false)
			{
				//�߰� �� �������� ���� or ���� ���콺�� �Ҵ�� ��ų ����

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
		//�Һ��� ������ ���� ��� ������ ���
		if (false)
		{

		}

		//����� ���� ��쿣 �ű�� ���� ���콺Ŭ���� �����ִ� ��ų ���
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

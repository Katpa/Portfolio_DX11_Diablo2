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
	//�������� Ŀ���� ��� �������� Ȯ���ؾ��Ѵ�
	//Ŀ���� ������ ���϶� ������ ������ ����� �Ǿ���Ѵ�.
	//�׷����� ������ ������ �������� ���ϰ� �־�� �Ѵ�.

	mouseAxis = CAM->ScreenToWorld(mousePos) + Vector2(10, -10);
	localPosition = mousePos + Vector2(10, -10);
	
	Control();

	if (pickingItem != nullptr)
		pickingItem->Update();

	AnimObject::Update();
}

void Cursor::Render()
{
	if (pickingItem != nullptr)
		pickingItem->Render();

	AnimObject::Render();
}

void Cursor::GetInventory(Inventory* inventory)
{
	this->inventory = inventory;
	overItem = inventory->ReturnOverItem();
}

void Cursor::Control()
{
	bool isLBtn = KEY_UP(VK_LBUTTON);
	bool isRBtn = KEY_UP(VK_RBUTTON);

	overItem = inventory->ReturnOverItem();
	
	if (uiManager->IsMouseOnStatus())
	{
		//�������ͽ� â ���� �浹ó�� �켱������ ó��
	}

	if (inventory->IsMouseOn())
	{
		if (overItem && !pickingItem)
		{
			if (isRBtn)
			{
				if (overItem->ReturnType() == Item::ItemType::CONSUMABLE)
				{
					player->AdaptItem(inventory->UseItem(overItem));
				}
				else
					inventory->AutoEquip(overItem);
			}
			else if (isLBtn)
			{
				overItem->HideInfo();
				pickingItem = overItem;
				overItem = nullptr;

				inventory->Picking(pickingItem);
			}
		}
		else if (pickingItem)
		{
			Vector2 startPos;
			if (inventory->IsAblePlaceItem(pickingItem, startPos))	//��� �ִ� �������� ���� ���콺 ��ǥ�� ������� ���� �κ��丮 ���Կ� �� �� �ִ��� Ȯ��
			{
				if (isLBtn)
				{
					inventory->PlaceItem(startPos, pickingItem);
					pickingItem = nullptr;
					converter->IsPicked() = false;
				}
			}
			else
			{
				if (isLBtn)
				{
					//�Ұ����ϴٴ� ������ �޼��� ����
				}
			}
		}
	}
	else if (pickingItem)
	{
		//�κ��丮 �ۿ��� ������ ��ŷ�� ����
	}


	if (uiManager->IsMouseOnBasicUI())
	{
		//���Ⱑ �⺻ UI
		
	}

	if (cain->IsMouseOn())
	{
		
		if (pickingItem)
		{
			//Ŀ���� ����� �������� ���� (�Ǹ�)
			if (isLBtn)
			{
				player->GetMoney(cain->SellItem(pickingItem));
				pickingItem = nullptr;
				inventory->IsPicked() = false;
			}
		}
		else if(cain->ReturnOverItem())
		{
			//Ŀ�� �Ʒ��� ������ �Ǵ� �������� ����(����)
			if (isRBtn)
			{
				//���ݰ� ������ ��
				if (player->SpendMoney(cain->ReturnOverItem()->ReturnCost()))
				{
					inventory->GetItem(cain->BuyItem());
				}
				else
				{
					//SFX->Play("noMoney", 0.1f);
				}
			}
		}
	}

	if (cain->IsMouseOnCain() && isLBtn)
	{
		int start = aStarPlus->FindCloseNode(player->GlobalPosition());
		int end = aStarPlus->FindCloseNode(mouseAxis);

		aStarPlus->GetPath(start, end, player->GetPath());

		player->ReturnNPC() = cain;
	}

	if (converter->IsMouseOn())
	{
		overItem = converter->ReturnOverItem();
		if (pickingItem)
		{
			Vector2 startPos;
			if (converter->IsAblePlaceItem(pickingItem, startPos))
			{
				if (isLBtn)
				{
					converter->PlaceItem(startPos, pickingItem);
					pickingItem = nullptr;
					inventory->IsPicked() = false;
				}
			}
		}
		else if(overItem)
		{
			//������ ����
			if (isLBtn)
			{
				overItem->HideInfo();
				pickingItem = overItem;
				overItem = nullptr;

				converter->Picking(pickingItem);
			}
		}
	}
	
	if (!inventory->IsMouseOn() && !uiManager->IsMouseOnStatus() &&
		!cain->IsMouseOn() && !cain->IsMouseOnCain() && !converter->IsMouseOn())
	{
		Monster* overMonster = MonsterManager::Get()->IsCollision(mouseAxis);

		if (isLBtn)
		{
			if (overMonster != nullptr)
			{
				//�߰� �� �������� ���� or ���� ���콺�� �Ҵ�� ��ų ����

			}
			else
			{
				int start = aStarPlus->FindCloseNode(player->GlobalPosition());
				int end = aStarPlus->FindCloseNode(mouseAxis);

				aStarPlus->GetPath(start, end, player->GetPath());
			}
		}
		else if (isRBtn)
		{
			//������ ��ų ���
			player->Cast(mouseAxis);
		}
		else if (KEY_DOWN(VK_MBUTTON))
			MonsterManager::Get()->SpawnMonster("CouncilMember", mouseAxis);
	}
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
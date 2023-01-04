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
	//아이템을 커서가 들고 있을때도 확인해야한다
	//커서가 아이템 위일때 아이템 정보가 출력이 되어야한다.
	//그러려면 아이템 정보를 아이템이 지니고 있어야 한다.

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
		//스테이터스 창 내의 충돌처리 우선적으로 처리
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
			if (inventory->IsAblePlaceItem(pickingItem, startPos))	//들고 있는 아이템이 현재 마우스 좌표에 기반으로 계산된 인벤토리 슬롯에 들어갈 수 있는지 확인
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
					//불가능하다는 음성과 메세지 띄우기
				}
			}
		}
	}
	else if (pickingItem)
	{
		//인벤토리 밖에서 아이템 피킹한 상태
	}


	if (uiManager->IsMouseOnBasicUI())
	{
		//여기가 기본 UI
		
	}

	if (cain->IsMouseOn())
	{
		
		if (pickingItem)
		{
			//커서가 집어든 아이템이 있음 (판매)
			if (isLBtn)
			{
				player->GetMoney(cain->SellItem(pickingItem));
				pickingItem = nullptr;
				inventory->IsPicked() = false;
			}
		}
		else if(cain->ReturnOverItem())
		{
			//커서 아래에 오버랩 되는 아이템이 있음(구매)
			if (isRBtn)
			{
				//가격과 소지금 비교
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
			//아이템 집기
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
				//추격 후 근접공격 시전 or 왼쪽 마우스에 할당된 스킬 시전

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
			//지정된 스킬 사용
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
#include "Framework.h"

Cain::Cain()
{
	wstring file = L"Textures/Character/cain/idle.png";
	Texture* texture = Texture::Add(file);
	Vector2 cutSize = texture->GetSize() / Vector2(16, 1);
	vector<Frame*> frames;

	for (int i = 0; i < 16; i++)
	{
		frames.push_back(new Frame(file, cutSize.x * i, 0, cutSize.x, cutSize.y));
	}
	clips[0] = new Clip(frames);
	curClip = clips[0];
	curClip->Play();

	collider = new RectCollider(cutSize);
	collider->SetParent(this);

	storeBoard = new Quad(L"Textures/UI/store.png");
	storeBoard->Position() = { CENTER_X - storeBoard->Size().x * 0.75f, CENTER_Y * 1.1f };
	storeBoard->UpdateWorld();
	storeBoard->Active() = false;

	storeCollider = new RectCollider(storeBoard->ReturnTexture()->GetSize());
	storeCollider->SetParent(storeBoard);
	storeCollider->UpdateWorld();

	closeButton = new UpDownButton(L"Textures/UI/btn_cancel_up.png", L"Textures/UI/btn_cancel_down.png");
	closeButton->SetParent(storeBoard);
	closeButton->Position() = { 127, -185 };
	closeButton->SetEvent(bind(&Cain::CloseStore, this));
	closeButton->Update();

	SetSlots();

	floatValueBuffer = new FloatValueBuffer();
	floatValueBuffer->data.values[0] = cutSize.x;
	floatValueBuffer->data.values[1] = cutSize.y;

	SetPixelShader(L"Shaders/PixelOutline.hlsl");
	GetColorBuffer()->Get() = { 1, 1, 1, 1 };

	ZOrder::Get()->Insert(this);

	InitItem();
}

Cain::~Cain()
{
	delete collider;
	delete storeBoard;
	delete closeButton;
	delete floatValueBuffer;
}

void Cain::Update()
{
	AnimObject::Update();
	collider->UpdateWorld();

	isMouseOn = false;
	isMouseOnCain = collider->IsPointCollision(CAM->ScreenToWorld(mousePos));

	if (isMouseOnCain)
	{
		floatValueBuffer->data.values[2] = 0.05f;
	}
	else
	{
		floatValueBuffer->data.values[2] = 0;
	}

	if (storeBoard->Active())
	{
		if (storeCollider->IsPointCollision(mousePos))
			isMouseOn = true;
		
		closeButton->Update();
		overItem = IsOverItem();

		for (Item* item : items)
		{
			item->Update();
		}
	}
}

void Cain::Render()
{
	floatValueBuffer->SetPS(2);
	AnimObject::Render();
}

void Cain::RenderUI()
{
	if (!storeBoard->Active()) return;

	storeBoard->Render();
	closeButton->Render();

	for (Item* item : items)
	{
		item->Render();
	}

	for (Item* item : items)
	{
		item->InfoRender();
	}
}

void Cain::OpenStore()
{
	storeBoard->Active() = true;
	SFX->Play("cainHello", 0.1f);
}

void Cain::CloseStore()
{
	storeBoard->Active() = false;
	SFX->Play("cainGoodbye", 0.1f);
	SFX->Play("button", 0.5f);
}

void Cain::SwitchStore()
{
	storeBoard->Active() = !storeBoard->Active();
}

Item* Cain::BuyItem()
{
	if (!overItem) return nullptr;

	ClearItemPos(overItem);

	SFX->Play("gold", 0.2f);

	return overItem;
}

int Cain::SellItem(Item* item)
{
	GetItem(item);

	SFX->Play("gold", 0.2f);

	return item->ReturnCost();
}

void Cain::SetSlots()
{
	//초기 위치 다시 잡아주기
	Vector2 startPos = storeBoard->Position() + Vector2(-130, 138);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			slots[i][j].pos.x = startPos.x + SLOT_SIZE * j;
			slots[i][j].pos.y = startPos.y - SLOT_SIZE * i;
			slots[i][j].isEmpty = true;
		}
	}
}

void Cain::GetItem(Item* item)
{
	Vector2 tmpPos;
	if (FindSpace(item->ReturnSize(), tmpPos))
		PlaceItem(tmpPos, item);
}

void Cain::PlaceItem(Vector2 startPos, Item* item)
{
	Vector2 itemSize = item->ReturnSize();
	Vector2 lastNum;

	item->ReturnInventoryPos().clear();

	for (int i = startPos.y; i < startPos.y + itemSize.y; i++)
	{
		for (int j = startPos.x; j < startPos.x + itemSize.x; j++)
		{
			slots[i][j].isEmpty = false;
			item->ReturnInventoryPos().push_back(Vector2(j, i));

			lastNum.x = j;
		}

		lastNum.y = i;
	}

	items.push_back(item);
	item->Position() = { (slots[(int)startPos.y][(int)startPos.x].pos + slots[(int)lastNum.y][(int)lastNum.x].pos) * 0.5f };

	//isPicked = false;
}

bool Cain::FindSpace(Vector2 itemSize, Vector2& itemPos)
{
	Vector2 output;

	int limitX = 10 - itemSize.x + 1;
	int limitY = 10 - itemSize.y + 1;

	for (int i = 0; i < limitY; i++)
	{
		int emptyCount = 0;

		for (int j = 0; j < limitX; j++)
		{
			if (slots[i][j].isEmpty)
			{
				emptyCount++;
			}
			else
			{
				emptyCount = 0;
			}

			if (emptyCount == itemSize.x)
			{
				if (IsPlace(Vector2(j - emptyCount + 1, i), itemSize))
				{
					output.x = j - emptyCount + 1;
					output.y = i;

					itemPos = output;

					return true;
				}
			}
		}
	}

	return false;
}

bool Cain::IsPlace(Vector2 startPos, Vector2 size)
{
	if (startPos.y + size.y > 10 || startPos.x + size.x > 10) return false;

	for (int i = startPos.y; i < startPos.y + size.y; i++)
	{
		for (int j = startPos.x; j < startPos.x + size.x; j++)
		{
			if (!slots[i][j].isEmpty)
				return false;
		}
	}

	return true;
}

Item* Cain::IsOverItem()
{
	Item* output = nullptr;

	for (Item* item : items)
	{
		if (item->IsCollision(mousePos))
		{
			output = item;
			item->ShowInfo();
		}
		else
		{
			item->HideInfo();
		}
	}

	return output;
}

void Cain::ClearItemPos(Item* item)
{
	//items에서 해당 아이템 제거
	for (int i = 0; i < items.size(); i++)
	{
		if (item == items[i])
		{
			items.erase(items.begin() + i);
			break;
		}
	}

	//해당 아이템이 가지고 있던 각 좌표의 isEmpty값 변경
	for (Vector2 vec : item->ReturnInventoryPos())
	{
		slots[(int)vec.y][(int)vec.x].isEmpty = true;
	}

	//해당 아이템이 가지고 있는 인벤토리포지션 제거
	item->ReturnInventoryPos().clear();
}

void Cain::InitItem()
{
	GetItem(ItemManager::Get()->GetItem("greatSword"));
	GetItem(ItemManager::Get()->GetItem("belt"));
	GetItem(ItemManager::Get()->GetItem("boots"));
	GetItem(ItemManager::Get()->GetItem("heavyArmor"));
	GetItem(ItemManager::Get()->GetItem("helm"));
	GetItem(ItemManager::Get()->GetItem("wand"));
	GetItem(ItemManager::Get()->GetItem("lightArmor"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("hpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
	GetItem(ItemManager::Get()->GetItem("mpPotion"));
}
#include "Framework.h"

Converter::Converter()
	:Quad(L"Textures/UI/converter.png")
{
	Position() = { CENTER_X - Size().x * 0.75f, CENTER_Y * 1.1f };
	UpdateWorld();
	isActive = false;

	InitSlots();
	InitList();

	convertButton = new UpDownButton(L"Textures/UI/btn_convert_up.png", L"Textures/UI/btn_convert_down.png");
	convertButton->SetEvent(bind(&Converter::Convert, this));
	convertButton->SetParent(this);
	convertButton->Position() = {0, -59};

	closeButton = new UpDownButton(L"Textures/UI/btn_cancel_up.png", L"Textures/UI/btn_cancel_down.png");
	closeButton->SetEvent(bind(&Converter::Close, this));
	closeButton->SetParent(this);
	closeButton->Position() = {131, -185};

	collider = new RectCollider(Size());
	collider->SetParent(this);
	collider->UpdateWorld();
	
	effect = new Effect(L"Textures/Effects/convert.png", Vector2(6, 5), 0.05f, true);
}

Converter::~Converter()
{
	delete convertButton;
	delete closeButton;
	delete collider;
	delete effect;
}

void Converter::Update()
{
	isMouseOn = false;

	if (!isActive) return;

	if (collider->IsPointCollision(mousePos))
		isMouseOn = true;

	if (!isPicked)
		overItem = IsOverItem();

	convertButton->Update();
	closeButton->Update();

	for (Item* item : items)
		item->Update();

	effect->Update();
}

void Converter::Render()
{
	if (!isActive) return;

	Quad::Render();
	convertButton->Render();
	closeButton->Render();

	for (Item* item : items)
		item->Render();

	for (Item* item : items)
		item->InfoRender();

	effect->Render();
}

void Converter::Picking(Item* item)
{
	ClearItemPos(item);
	item->Picked();
	isPicked = true;
	overItem = nullptr;
}

bool Converter::IsAblePlaceItem(Item* item, Vector2& startPos)
{
	Vector2 tmpPos;
	float distance = 200;
	POINT closerSlot;
	POINT size;
	size.x = (int)item->ReturnSize().x;
	size.y = (int)item->ReturnSize().y;



	tmpPos = { mousePos.x - HALF_SLOT_SIZE * (size.x - 1),
				 mousePos.y + HALF_SLOT_SIZE * (size.y - 1) };

	//여기서 startPos와 가장 가까운 위치의 칸을 찾는다.

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vector2 tmp = slots[i][j].pos - tmpPos;

			if (distance > tmp.Length())
			{
				distance = tmp.Length();
				closerSlot = { j, i };
			}
		}
	}

	if (IsPlace(Vector2(closerSlot.x, closerSlot.y), item->ReturnSize()))
	{
		//여기는 초록색 투영
		startPos.x = closerSlot.x;
		startPos.y = closerSlot.y;

		return true;
	}
	else
	{
		//여기는 빨간색 투영

		return false;
	}
}

void Converter::InitSlots()
{
	Vector2 startPos = Position() + Vector2(-28, 62);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			slots[i][j].pos.x = startPos.x + SLOT_SIZE * j;
			slots[i][j].pos.y = startPos.y - SLOT_SIZE * i;
			slots[i][j].isEmpty = true;
		}
	}
}

void Converter::InitList()
{
	convertList.resize(2);

	//조합1
	convertList[0].push_back("lightSaber");
	convertList[0].push_back("greatSword");
	convertList[0].push_back("wand");

	//조합2
	convertList[1].push_back("hmpPotion");
	convertList[1].push_back("hpPotion");
	convertList[1].push_back("mpPotion");
}

void Converter::Convert()
{
	convertItemKey = "";

	if (CheckItems())
	{
		//모든 조합 재료 아이템 제거
		for (Item* item : items)
		{
			item->Active() = false;
			ClearItemPos(item);
		}

		//조합 아이템 생성
		GetItem(ItemManager::Get()->GetItem(convertItemKey));

		effect->Play(Position() + Vector2(0, 15));
		SFX->Play("convert", 0.1f);
	}
}

bool Converter::CheckItems()
{
	//아이템 벡터에서 첫 아이템을 뽑아서 키값을 가져오고
	//그 키값으로 조합법에서 일치하는 항목이 있는지 찾는다
	//그 항목에서 아이템 벡터의 다음 아이템의 키값을 비교하여
	//일치하는 경우 나머지 항목은 제외시키고
	//이같은 과정을 반복하여 남은 리스트 항목이 한개가 될때까지 반복한다

	if (items.size() > 2 || items.empty()) return false;

	vector<int> index;
	for (int i = 0; i < convertList.size(); i++)
	{
		index.push_back(i);
	}

	if (!CheckList(items[0]->ReturnKey(), index)) return false;

	if (!CheckList(items[1]->ReturnKey(), index)) return false;

	convertItemKey = convertList[index[0]][0];

	return true;
}

bool Converter::CheckList(string key, vector<int>& index)
{
	int j = 0;
	for (int i = 0; i < index.size(); i++)
	{
		if (key != convertList[index[i]][1] && key != convertList[index[i]][2])
		{
			index.erase(index.begin() + i);
			i--;
		}
		j++;
	}
	
	if (index.empty())
		return false;

	return true;
}

void Converter::GetItem(Item* item)
{
	Vector2 tmpPos;
	if (FindSpace(item->ReturnSize(), tmpPos))
		PlaceItem(tmpPos, item);
}

void Converter::PlaceItem(Vector2 startPos, Item* item)
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

	isPicked = false;
}

bool Converter::FindSpace(Vector2 itemSize, Vector2& itemPos)
{
	Vector2 output;

	int limitX = 3 - itemSize.x + 1;
	int limitY = 4 - itemSize.y + 1;

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

bool Converter::IsPlace(Vector2 startPos, Vector2 size)
{
	if (startPos.y + size.y > 4 || startPos.x + size.x > 3) return false;

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

Item* Converter::IsOverItem()
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

void Converter::ClearItemPos(Item* item)
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

void Converter::Close()
{
	isActive = false;
	SFX->Play("button", 0.5f);
}
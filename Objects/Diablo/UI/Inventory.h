#pragma once

/// <summary>
/// 1. 기본 배경이미지 출력
/// 2. 그 위의 각 칸의 좌표 잡아서 지정해주기
/// 3. 장비칸에는 해당 장비가 맞는지 확인하기
/// 4. 장비를 획득하면 알맞는 칸에 배치할 수 있게 검사해야한다
/// 5. 인벤토리에서 갖고있는 아이템 목록이 필요할까?
/// 6. 아이템은 마우스 포인트랑 충돌처리를 해야한다
/// 7. 
/// </summary>

struct ItemSlot
{
	Vector2 pos;
	bool isEmpty;
};

class Inventory : public Quad
{
private:
	struct EquipSlot
	{
		Item* item = nullptr;
		Quad* bg;

		void Update()
		{
			if(item != nullptr)
				item->Update();
			bg->UpdateWorld();
		}

		void Equip(Item* item)
		{
			this->item = item;
			this->item->Position() = bg->GlobalPosition();
		}

		void Unequip(Item* item)
		{
			if (this->item == item)
				this->item = nullptr;
		}

		void IsOver(Item*& output, Vector2 mousePos)
		{
			if (!item) return;

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

		POINT Effect()
		{
			if (item == nullptr)
				return POINT();
			else
				return item->ReturnEffectPower();
		}

		void Render()
		{
			if (item != nullptr)
				item->Render();
			else
				bg->Render();
		}

		void InfoRender()
		{
			if (item != nullptr)
				item->InfoRender();
		}

		void Delete()
		{
			delete bg;
		}
	};

	struct EquipSlots
	{
		EquipSlot helm;
		EquipSlot weaponL;
		EquipSlot weaponR;
		EquipSlot armor;
		EquipSlot belt;
		EquipSlot glove;
		EquipSlot boots;

		void Update()
		{
			helm.Update();
			weaponL.Update();
			weaponR.Update();
			armor.Update();
			belt.Update();
			glove.Update();
			boots.Update();
		}

		void Render()
		{
			helm.Render();
			weaponL.Render();
			weaponR.Render();
			armor.Render();
			belt.Render();
			glove.Render();
			boots.Render();

			helm.InfoRender();
			weaponL.InfoRender();
			weaponR.InfoRender();
			armor.InfoRender();
			belt.InfoRender();
			glove.InfoRender();
			boots.InfoRender();
		}

		void Delete()
		{
			helm.Delete();
			weaponL.Delete();
			weaponR.Delete();
			armor.Delete();
			belt.Delete();
			glove.Delete();
			boots.Delete();
		}

		void IsOverItem(Item*& output, Vector2 mousePos)
		{
			helm.IsOver(output, mousePos);
			weaponL.IsOver(output, mousePos);
			weaponR.IsOver(output, mousePos);
			armor.IsOver(output, mousePos);
			belt.IsOver(output, mousePos);
			glove.IsOver(output, mousePos);
			boots.IsOver(output, mousePos);
		}

		void Unequip(Item* item)
		{
			helm.Unequip(item);
			weaponL.Unequip(item);
			weaponR.Unequip(item);
			armor.Unequip(item);
			belt.Unequip(item);
			glove.Unequip(item);
			boots.Unequip(item);
		}
	};

	const float HALF_SLOT_SIZE = 14.5f;

public:
	Inventory(int* gold);
	~Inventory();

	void Update();
	void Render();

	void GetItem(Item* item);
	POINT UseItem(Item* item);
	void AutoEquip(Item* item);

	void Picking(Item* item);
	bool IsAblePlaceItem(Item* item, Vector2& startPos);					//마우스를 기반으로 아이템을 놓을 좌표를 계산하고 그 좌표 위에 아이템이 있는지 없는지 확인하는 함수
	void PlaceItem(Vector2 startPos, Item* item);

	void SetEmpty(UINT col, UINT row) { itemSlot[col][row].isEmpty = true; }
	void SetAllocated(UINT col, UINT row) { itemSlot[col][row].isEmpty = false; }

	POINT ReturnEquipEffect();
	bool IsMouseOn() { return isMouseOn; }
	Item*& ReturnOverItem() { return overItem; }
	bool& IsPicked() { return isPicked; }

private:
	bool FindSpace(Vector2 itemSize, Vector2& itemPos);

	bool IsPlace(Vector2 startPos, Vector2 size);

	void ClearItemPos(Item* item);

	void Close();

	Item* IsOverItem();
	
	void CheckItmeCollide();				//인벤토리 내부의 아이템들 사이에 충돌이 나는지 확인하기 충돌이면 그 아이템들 싹다 빨강으로 
											//대신 체크하는건 pick상태인 아이템만 하면 돼
	void InitEquip();
	void InitItemSlot();

private:
	ItemSlot itemSlot[4][10];
	ItemSlot quickSlot[1][4];
	
	EquipSlots equip;
	vector<Item*> items;

	//Equipment* tmpItem;

	Item* overItem = nullptr;
	UpDownButton* closeButton;

	int* gold;

	bool isMouseOn = false;
	bool isPicked = false;
};
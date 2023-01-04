#pragma once

/// <summary>
/// 1. �⺻ ����̹��� ���
/// 2. �� ���� �� ĭ�� ��ǥ ��Ƽ� �������ֱ�
/// 3. ���ĭ���� �ش� ��� �´��� Ȯ���ϱ�
/// 4. ��� ȹ���ϸ� �˸´� ĭ�� ��ġ�� �� �ְ� �˻��ؾ��Ѵ�
/// 5. �κ��丮���� �����ִ� ������ ����� �ʿ��ұ�?
/// 6. �������� ���콺 ����Ʈ�� �浹ó���� �ؾ��Ѵ�
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
	bool IsAblePlaceItem(Item* item, Vector2& startPos);					//���콺�� ������� �������� ���� ��ǥ�� ����ϰ� �� ��ǥ ���� �������� �ִ��� ������ Ȯ���ϴ� �Լ�
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
	
	void CheckItmeCollide();				//�κ��丮 ������ �����۵� ���̿� �浹�� ������ Ȯ���ϱ� �浹�̸� �� �����۵� �ϴ� �������� 
											//��� üũ�ϴ°� pick������ �����۸� �ϸ� ��
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
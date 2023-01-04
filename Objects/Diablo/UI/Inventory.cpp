#include "Framework.h"

Inventory::Inventory(int* gold)
    :Quad(L"Textures/UI/Inventory.png")
{
    Position() = { CENTER_X + Size().x * 0.75f, CENTER_Y * 1.1f };

    closeButton = new UpDownButton(L"Textures/UI/btn_cancel_up.png", L"Textures/UI/btn_cancel_down.png");
    closeButton->SetParent(this);
    closeButton->Position() = {-127, -185};
    closeButton->SetEvent(bind(&Inventory::Close, this));

    InitEquip();

    InitItemSlot();

    this->gold = gold;
}

Inventory::~Inventory()
{
    delete closeButton;
    equip.Delete();

    //delete tmpItem;
}

void Inventory::Update()
{
    isMouseOn = false;

    if (!isActive) return;

    equip.Update();

    Vector2 TopRight = Position() + Size() * 0.5f;
    Vector2 BotLeft = Position() - Size() * 0.5f;

    if (mousePos.x < TopRight.x && mousePos.y < TopRight.y &&
        mousePos.x > BotLeft.x && mousePos.y > BotLeft.y)
    {
        isMouseOn = true;
    }

    if(!isPicked)
        overItem = IsOverItem();

    UpdateWorld();
    closeButton->Update();

    for (Item* item : items)
        item->Update();
}

void Inventory::Render()
{
    if (!isActive) return;

    Quad::Render();
    closeButton->Render();
    equip.Render();

    string money = to_string(*gold);
    Font::Get()->RenderText(money, "status", Position() - Vector2(20, 185));

    for (Item* item : items)
        item->Render();

    for (Item* item : items)
        item->InfoRender();
}

void Inventory::GetItem(Item* item)
{
    Vector2 tmpPos;
    if (FindSpace(item->ReturnSize(), tmpPos))
        PlaceItem(tmpPos, item);
}

POINT Inventory::UseItem(Item* item)
{
    ClearItemPos(item);
    return item->Use();
}

void Inventory::AutoEquip(Item* item)
{
    Item* swapItem = nullptr;

    item->HideInfo();

    switch (item->ReturnType())
    {
    case Item::HELM:
        if (equip.helm.item)
            swapItem = equip.helm.item;
        equip.helm.Equip(item);
        break;
    case Item::ARMOR:
        if (equip.armor.item)
            swapItem = equip.armor.item;
        equip.armor.Equip(item);
        break;
    case Item::HANDL:
        if (equip.weaponL.item)
            swapItem = equip.weaponL.item;
        equip.weaponL.Equip(item);
        break;
    case Item::HANDR:
        if (equip.weaponR.item)
            swapItem = equip.weaponR.item;
        equip.weaponR.Equip(item);
        break;
    case Item::GLOVE:
        if (equip.glove.item)
            swapItem = equip.glove.item;
        equip.glove.Equip(item);
        break;
    case Item::BELT:
        if (equip.belt.item)
            swapItem = equip.belt.item;
        equip.belt.Equip(item);
        break;
    case Item::BOOTS:
        if (equip.boots.item)
            swapItem = equip.boots.item;
        equip.boots.Equip(item);
        break;
    }

    ClearItemPos(item);

    //이미 착용하고 있던 아이템이 있을 경우에 인벤토리에 알맞은 영역에 넣어주거나 없다면 바닥에 흘린다
    if (swapItem)
    {
        Vector2 swapPos;
        if (FindSpace(swapItem->ReturnSize(), swapPos))
            PlaceItem(swapPos, swapItem);
    }
}

void Inventory::Picking(Item* item)
{
    ClearItemPos(item);
    item->Picked();
    isPicked = true;
    overItem = nullptr;
}

bool Inventory::IsAblePlaceItem(Item* item, Vector2& startPos)
{
    // 1. 마우스는 좌표들의 가장 중앙으로 한다. = 마우스 위치로 각 칸을 잡을 수 있어야 한다
    //      ->아이템 사이즈로 해서 범위를 잡는 식으로 한다.
    //      ->마우스 기준에서 사이즈 크기에 비례해 아이템의 좌측상단의 좌표가 될 좌표를 계산한다
    //          -x의 경우 1은 같은 위치 / 2는 반칸 좌로
    //          -y의 경우 1은 같은 위치 / 2는 반칸 / 3은 한칸 / 4는 한칸 반 위로
    //      ->그 좌표에서 가장 가까운 칸을 찾는다
    //      ->그리고 그 칸 기준으로 들어갈 수 있는지 확인한다

    Vector2 tmpPos;
    float distance = 200;
    POINT closerSlot;
    POINT size;
    size.x = (int)item->ReturnSize().x;
    size.y = (int)item->ReturnSize().y;

    

    tmpPos = { mousePos.x - HALF_SLOT_SIZE * (size.x - 1),
                 mousePos.y + HALF_SLOT_SIZE * (size.y - 1)};

    //여기서 startPos와 가장 가까운 위치의 칸을 찾는다.
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            Vector2 tmp = itemSlot[i][j].pos - tmpPos;

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

//가장 왼쪽 위 인벤 좌표를 받고 아이템을 배치해주는 함수
void Inventory::PlaceItem(Vector2 startPos, Item* item)
{
    Vector2 itemSize = item->ReturnSize();
    Vector2 lastNum;

    item->ReturnInventoryPos().clear();

    for (int i = startPos.y; i < startPos.y + itemSize.y; i++)
    {
        for (int j = startPos.x; j < startPos.x + itemSize.x; j++)
        {
            itemSlot[i][j].isEmpty = false;
            item->ReturnInventoryPos().push_back(Vector2(j, i));

            lastNum.x = j;
        }

        lastNum.y = i;
    }

    items.push_back(item);
    item->Position() = { (itemSlot[(int)startPos.y][(int)startPos.x].pos + itemSlot[(int)lastNum.y][(int)lastNum.x].pos) * 0.5f };

    isPicked = false;
}

//입력받은 아이템의 정보에 의해 배치 가능한 영역 탐색 후 가장 왼쪽 위의 좌표를 내보내는 함수
bool Inventory::FindSpace(Vector2 itemSize, Vector2& itemPos)
{
    // 이함수도 overlap처럼 참조형으로 하나 받아서 굳이 반환하지 않고 값을 넣어주고
    // 반환형은 bool형으로 할까??

    Vector2 output;

    int limitX = 10 - itemSize.x + 1;
    int limitY = 4 - itemSize.y + 1;


    for (int i = 0; i < limitY; i++)
    {
        int emptyCount = 0;

        for (int j = 0; j < limitX; j++)
        {
            if (itemSlot[i][j].isEmpty)
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

//첫 위치와 아이템 크기를 입력 받아서 해당 아이템을 배치 가능한지 확인해주는 함수
bool Inventory::IsPlace(Vector2 startPos, Vector2 size)
{
    if (startPos.y + size.y > 4 || startPos.x + size.x > 10) return false;

    for (int i = startPos.y; i < startPos.y + size.y; i++)
    {
        for (int j = startPos.x; j < startPos.x + size.x; j++)
        {
            if (!itemSlot[i][j].isEmpty)
                return false;
        }
    }

    return true;
}

void Inventory::ClearItemPos(Item* item)
{
    bool isEquip = true;

    //items에서 해당 아이템 제거
    for (int i = 0; i < items.size(); i++)
    {
        if (item == items[i])
        {
            items.erase(items.begin() + i);
            isEquip = false;
            break;
        }
    }

    if (isEquip)
    {
        //장비에서 제거
        equip.Unequip(item);
    }
    else
    {
        //해당 아이템이 가지고 있던 각 좌표의 isEmpty값 변경
        for (Vector2 vec : item->ReturnInventoryPos())
        {
            itemSlot[(int)vec.y][(int)vec.x].isEmpty = true;
        }

        //해당 아이템이 가지고 있는 인벤토리포지션 제거
        item->ReturnInventoryPos().clear();
    }
}

void Inventory::Close()
{
    isActive = false;
    SFX->Play("button", 0.5f);
}

Item* Inventory::IsOverItem()
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

    if (!output)
        equip.IsOverItem(output, mousePos);

    return output;
}

void Inventory::CheckItmeCollide()
{
}

void Inventory::InitEquip()
{
    equip.helm.bg = new Quad(L"Textures/UI/helmSlot.png");
    equip.helm.bg->SetParent(this);
    equip.helm.bg->Position() = { 2, 183 };
    equip.armor.bg = new Quad(L"Textures/UI/armorSlot.png");
    equip.armor.bg->SetParent(this);
    equip.armor.bg->Position() = { 2, 100 };
    equip.weaponL.bg = new Quad(L"Textures/UI/weaponSlot.png");
    equip.weaponL.bg->SetParent(this);
    equip.weaponL.bg->Position() = { -114, 115 };
    equip.weaponR.bg = new Quad(L"Textures/UI/weaponSlot.png");
    equip.weaponR.bg->SetParent(this);
    equip.weaponR.bg->Position() = { 117, 115 };
    equip.belt.bg = new Quad(L"Textures/UI/beltSlot.png");
    equip.belt.bg->SetParent(this);
    equip.belt.bg->Position() = { 2, 28 };
    equip.glove.bg = new Quad(L"Textures/UI/gloveSlot.png");
    equip.glove.bg->SetParent(this);
    equip.glove.bg->Position() = { -113, 13 };
    equip.boots.bg = new Quad(L"Textures/UI/bootsSlot.png");
    equip.boots.bg->SetParent(this);
    equip.boots.bg->Position() = { 118, 13 };
}

void Inventory::InitItemSlot()
{
    Vector2 startPos = { CENTER_X + 112, 342 };

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            itemSlot[i][j].pos.x = startPos.x + SLOT_SIZE * j;
            itemSlot[i][j].pos.y = startPos.y - SLOT_SIZE * i;
            itemSlot[i][j].isEmpty = true;
        }
    }
}

POINT Inventory::ReturnEquipEffect()
{
    POINT output;
    output.x = equip.weaponL.Effect().x + equip.weaponR.Effect().x;
    output.y = equip.armor.Effect().y + equip.belt.Effect().y + equip.boots.Effect().y + equip.helm.Effect().y + equip.glove.Effect().y;

    return output;
}

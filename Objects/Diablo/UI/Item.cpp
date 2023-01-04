#include "Framework.h"

Item::Item(string key, wstring file, wstring info, ItemType type, Vector2 size, ItemEffect effect)
	:Quad(file), key(key), type(type), size(size), effect(effect)
{
	collider = new RectCollider(this->Size());
	collider->SetParent(this);

	this->info = new Quad(info);
	//this->info->SetParent(this);

	int resize = (int)(size.x * size.y);

	inventoryPos.resize(resize);
}

Item::~Item()
{
	delete collider;
	delete info;
}

void Item::Update()
{
	if (!isActive) return;

	switch (state)
	{
	case Item::ItemState::NONE:
		GetColorBuffer()->Set(colors.none);
		break;
	case Item::ItemState::OVER:
		GetColorBuffer()->Set(colors.green);
		break;
	case Item::ItemState::PICK:
		Position() = mousePos;
		GetColorBuffer()->Set(colors.picked);
		break;
	case Item::ItemState::COLLIDE:
		GetColorBuffer()->Set(colors.red);
		break;
	}

	UpdateWorld();
	collider->UpdateWorld();
	if(info->Active())
		info->UpdateWorld();
}

void Item::Render()
{
	if (!isActive) return;

	Quad::Render();
}

void Item::InfoRender()
{
	if (!isActive) return;

	if (info->Active())
		info->Render();
}

void Item::Picked()
{
	state = ItemState::PICK;
}

void Item::ShowInfo()
{
	info->Active() = true;
	state = ItemState::OVER;

	if (Position().y > (Size().y + info->Size().y) * 0.5f)
	{
		info->Position().y = Position().y - (Size().y + info->Size().y) * 0.5f;
	}
	else
	{
		info->Position().y = Position().y + (Size().y + info->Size().y) * 0.5f;
	}

	if (Position().x < WIN_WIDTH - info->Size().x * 0.5f)
	{
		info->Position().x = Position().x;
	}
	else
	{
		info->Position().x = WIN_WIDTH - info->Size().x * 0.5f;
	}

	info->UpdateWorld();
}

void Item::HideInfo()
{
	info->Active() = false;
	state = ItemState::NONE;
}

#include "Framework.h"

UpDownButton::UpDownButton(wstring file)
{
	Texture* texture = Texture::Add(file);
	Vector2 cutSize = texture->GetSize() / Vector2(2, 1);

	up = new Frame(file, cutSize.x * 0, 0, cutSize.x, cutSize.y);
	down = new Frame(file, cutSize.x * 1, 0, cutSize.x, cutSize.y);
	curFrame = up;

	collider = new RectCollider(cutSize);
}

UpDownButton::~UpDownButton()
{
	delete up;
	delete down;
	delete collider;
}

void UpDownButton::Update()
{
}

void UpDownButton::Render()
{
	curFrame->Render();

	if (false)
		collider->Render();
}

void UpDownButton::SetUP()
{
	if (Event != nullptr)
		Event();
}

void UpDownButton::SetPress()
{
}

void UpDownButton::SetDown()
{
}

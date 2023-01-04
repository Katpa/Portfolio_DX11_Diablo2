#include "Framework.h"

IButton::IButton(wstring file, Vector2 size)
	:Isometric(file)
{
	collider = new IsometricCollider(size);
	collider->SetParent(this);
}

IButton::IButton(wstring file)
	:Isometric(file)
{
	collider = new IsometricCollider(Size());
	collider->SetParent(this);
}

IButton::IButton(Vector2 size)
	:Isometric(size)
{
	collider = new IsometricCollider(size);
	collider->SetParent(this);
}

IButton::~IButton()
{
	delete collider;
}

void IButton::Update(bool isDebugMode)
{
	if (this == nullptr) return;

	if (isDebugMode)
	{
		if (collider->IsPointCollision(mousePos))
		{
			if (KEY_DOWN(VK_LBUTTON))
				isDownCheck = true;

			if (KEY_PRESS(VK_LBUTTON))
				state = DOWN;
			else
				state = OVER;

			if (isDownCheck && KEY_UP(VK_LBUTTON))
			{
				if (Event != nullptr)
					Event();

				if (IntEvent != nullptr)
					IntEvent(intParam);

				isDownCheck = false;
			}
		}
		else
		{
			state = NONE;

			if (KEY_UP(VK_LBUTTON))
				isDownCheck = false;
		}
	}

	switch (state)
	{
	case IButton::NONE:
		GetColorBuffer()->Set(noneColor);
		break;
	case IButton::DOWN:
		GetColorBuffer()->Set(downColor);
		break;
	case IButton::OVER:
		GetColorBuffer()->Set(overColor);
		break;
	}

	UpdateWorld();
	collider->UpdateWorld();
}

void IButton::Render()
{
	Isometric::Render();

	//if(isDebugMode)
	//	collider->Render();
}

#include "Framework.h"

UpDownButton::UpDownButton(wstring file1, wstring file2)
{
	up = new Quad(file1);
	up->SetParent(this);
	down = new Quad(file2);
	down->SetParent(this);
	curImage = up;

	collider = new RectCollider(up->Size());
	collider->SetParent(this);
}

UpDownButton::~UpDownButton()
{
	delete up;
	delete down;
	delete collider;
}

void UpDownButton::Update()
{
	if (!isActive) return;

	//1. key_down이면 이미지가 바뀌고
	//2. key_up일 때 저장된 함수가 실행되야 한다
	//3. 그와 동시에 이미지가 up으로 바뀌어야한다
	//4. 그리고 단축키로 눌러서 버튼이 작동해야한다

	if ((KEY_DOWN(VK_LBUTTON) && collider->IsPointCollision(mousePos)) ||
		 KEY_DOWN(hotKey))
	{
		SetDown();
	}

	if ((KEY_UP(VK_LBUTTON) && collider->IsPointCollision(mousePos)) ||
		 KEY_UP(hotKey))
	{
		if (Event != nullptr)
			Event();

		SetUp();
	}

	UpdateWorld();
	curImage->UpdateWorld();
	collider->UpdateWorld();
}

void UpDownButton::Render()
{
	if (!isActive) return;

	curImage->Render();

	if (false)
		collider->Render();
}

void UpDownButton::SetDown()
{
	isDown = true;

	curImage = down;
}

void UpDownButton::SetUp()
{
	isDown = false;

	curImage = up;
}

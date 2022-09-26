#pragma once

class UpDownButton : public Transform
{
private:
	enum State
	{
		UP, DOWN
	};

public:
	UpDownButton(wstring file);
	~UpDownButton();

	void Update();
	void Render();

	void Hide();
	void Show();

	void SetUP();
	void SetPress();
	void SetDown();

	void SetEvent(CallBack Event) { this->Event = Event; }

private:
	Frame* curFrame;
	Frame* up;
	Frame* down;

	RectCollider* collider;
	State state = UP;

	CallBack Event = nullptr;


};
#pragma once

class UpDownButton : public Transform
{
public:
	UpDownButton(wstring file1, wstring file2);
	~UpDownButton();

	void Update();
	void Render();

	void SetEvent(CallBack Event) { this->Event = Event; }
	void SetHotKey(char key) { hotKey = key; }

private:
	void SetDown();
	void SetUp();

private:
	Quad* curImage;
	Quad* up;
	Quad* down;

	char hotKey = VK_PAUSE;

	RectCollider* collider;
	bool isDown = false;

	CallBack Event = nullptr;
};
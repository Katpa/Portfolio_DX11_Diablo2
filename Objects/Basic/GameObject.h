#pragma once

class GameObject : public Transform
{
public:
	GameObject();
	~GameObject();

	virtual void Render();
	virtual float GetDepth() { return localPosition.y; }
};
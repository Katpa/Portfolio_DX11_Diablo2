#pragma once

class GameObject : public Transform
{
public:
	GameObject();
	~GameObject();

	virtual void Render();
	virtual float GetDepth() { return GlobalPosition().y; }
	virtual float GetPosX() { return GlobalPosition().x; }
};
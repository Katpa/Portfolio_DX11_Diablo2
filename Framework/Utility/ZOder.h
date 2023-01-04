#pragma once

class ZOrder : public Singleton<ZOrder>
{
public:
	ZOrder();
	~ZOrder();

	void Render();

	void Insert(class GameObject* obj);

	static int CompareObj(GameObject* obj1, GameObject* obj2);

private:
	vector<GameObject*> gameObjects;
};
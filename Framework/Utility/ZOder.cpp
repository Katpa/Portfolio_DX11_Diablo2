#include "Framework.h"

ZOrder::ZOrder()
{
}

ZOrder::~ZOrder()
{
}

void ZOrder::Render()
{
	//Z-Order를 위한 정렬
	sort(gameObjects.begin(), gameObjects.end(), ZOrder::CompareObj);

	float R = CAM->GlobalPosition().x + WIN_WIDTH + 100;
	float L = CAM->GlobalPosition().x - 100;
	float T = CAM->GlobalPosition().y + WIN_HEIGHT + 100;
	float B = CAM->GlobalPosition().y - 100;

	for (GameObject* obj : gameObjects)
	{
		if (!obj->Active()) continue;

		//프러스텀 컬링 (Frustum Culling)
		if (obj->GetPosX() < R && obj->GetPosX() > L &&
			obj->GetDepth() < T && obj->GetDepth() > B)
		{
			obj->Render();
		}
	}
}

void ZOrder::Insert(GameObject* obj)
{
	gameObjects.push_back(obj);
}

int ZOrder::CompareObj(GameObject* obj1, GameObject* obj2)
{
	return obj1->GetDepth() > obj2->GetDepth();
}

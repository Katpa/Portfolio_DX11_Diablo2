#pragma once

class AStarPlus : public AStar
{
public:
	AStarPlus(class InstancingMap* map);
	~AStarPlus();

	void SetRender();

private:
	virtual void SetEdge(UINT width);
	vector<Matrix> instances;
};
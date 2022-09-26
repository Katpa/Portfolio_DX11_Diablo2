#pragma once

class AStarPlus : public AStar
{
public:
	AStarPlus(GameTileMap* map);
	~AStarPlus();

private:
	virtual void SetEdge(UINT width);
};
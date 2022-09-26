#pragma once

class DiabloScene : public Scene
{
public:
	DiabloScene();
	~DiabloScene();

	virtual void Update() override;
	virtual void Render() override;
	
	void DebugControl();
	void TempTileMapControl();

private:
	GameTileMap* gameTileMap;
	Cursor* cursor;

	AStar* astar;
	AStarPlus* astarPlus;

	bool isDebugMode = false;
	bool isShowPath = false;
};
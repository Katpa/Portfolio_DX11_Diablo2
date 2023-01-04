#pragma once

class DiabloScene : public Scene
{
public:
	DiabloScene();
	~DiabloScene();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void DebugControl();

private:
	void InitFX();
	void SetSFX();

private:
	InstancingMap* map;
	Cursor* cursor;

	Playable* player;
	Cain* cain;

	AStar* astar;
	AStarPlus* astarPlus;

	MiniMap* miniMap;
	bool isMiniMap = false;

	bool isDebugMode = false;
	bool isShowPath = false;
};
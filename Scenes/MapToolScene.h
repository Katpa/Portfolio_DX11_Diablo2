#pragma once

class MapToolScene : public Scene
{
public:
	MapToolScene();
	~MapToolScene();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void Save();
	void Load();

private:
	InstancingTileMap* map;
	TilePallet* pallet;

	string projectPath;
};
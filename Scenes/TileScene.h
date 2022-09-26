#pragma once

class TileScene : public Scene
{
public:
	TileScene();
	~TileScene();
		
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void CreateSample();

	void ClickSample(int index);

	void Save();
	void Load();
private:
	int selectSampleNum = 0;
	Tile::Type selectType = Tile::BG;

	vector<Button*> sampleBtns;
	vector<wstring> sampleFiles;

	TileMap* tileMap;

	string projectPath;

	bool isDebugMode = false;
};
#pragma once

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	virtual void Update() override;
	virtual void Render() override;

private:
	InstancingTileMap* map;
};
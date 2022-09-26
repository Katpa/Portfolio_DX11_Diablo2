#pragma once

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Update();
	void Render();

private:
	void Create();
	void Delete();

private:
	Scene* scene;

	float renderTime = 0;
	const float renderDelayTime = 0.0069f;
};
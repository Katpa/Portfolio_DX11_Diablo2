#pragma once

class GameTileMap : public Transform
{
private:
	const float PUSH_SPEED = 500.0f;

	enum Direction
	{
		UP, DOWN, RIGHT, LEFT, NONE
	};

public:
	GameTileMap(string file);
	~GameTileMap();

	void Update(bool isDebugMode);
	void Render(bool isDebugMode);

	void PushTank(class Tank* tank);

	void GetNodes(vector<Node*>& nodes);

	UINT GetWidth() { return width; }
	Vector2 Size() { return targetTexture->Size() * 0.5f; }
private:
	void Load(string file);

	void DrawBGTile();

	Direction GetDirection(Transform* tile, Transform* target);
private:
	UINT width, height;

	Vector2 tileSize;

	vector<Tile*> bgTiles;
	vector<Tile*> objTiles;

	RenderTarget* renderTarget;
	Quad* targetTexture;
};
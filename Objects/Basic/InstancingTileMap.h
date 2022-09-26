#pragma once

class InstancingTileMap : public Transform
{
public:
	struct InstanceData
	{
		Matrix transform;
		Vector2 maxFrame;
		Vector2 curFrame;
		Float4 color;
	};

	enum BtnState
	{
		NONE, DOWN, OVER
	};


	enum BatchType
	{
		ISOMETRIC, RECT
	};

public:
	InstancingTileMap(wstring file, Vector2 maxFrame, UINT amount, UINT width, UINT height);	//Rect
	InstancingTileMap(wstring file, Vector2 maxFrame, UINT mapSize);							//Isometric
	~InstancingTileMap();

	void Update();
	void Render(bool isDebugMode);

	void ClickedTile();
	void SetUp();
	void SetDown();

	void Save(string file);
	void Load(string file);

private:
	void CreateTile();

	void SetTexture(UINT frameNum);
	UINT GetTexture(Vector2 pos);

	void MoveMapPos(Vector2 movePos);
	void InitMapPos();

private:
	Quad* quad;

	Vector2 maxFrame;
	Vector2 frameSize;
	Vector2 mapSize;

	vector<InstanceData> instances;
	vector<Collider*> colliders;
	vector<BtnState> states;
	vector<class Tile*> objTiles;

	VertexBuffer* instanceBuffer;

	Float4 noneColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Float4 downColor = { 0.0f, 0.0f, 0.7f, 1.0f };
	Float4 overColor = { 0.8f, 0.8f, 0.8f, 1.0f };

	BatchType type;
};
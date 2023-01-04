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

private:
	float MAP_MULTI_FACTOR = 1.5f;

public:
	InstancingTileMap(wstring file, Vector2 maxFrame, UINT mapSize);							//Isometric
	~InstancingTileMap();

	void Update();
	void Render(bool isDebugMode);

	void ClickedTile();

	void Save(string file);
	void Load(string file);

	Vector2 ReturnMapSize() { return mapSize; }
	UINT* ReturnSelectedTileNum() { return &selectedTileNum; }
	wstring* ReturnSelectedObj() { return &selectedObj; }

private:
	void DeleteAllTiles();

	void DeleteObject();
	void SetBgTile(UINT index);
	void SetObjTile(UINT index);

private:
	Quad* quad;

	Vector2 maxFrame;
	Vector2 frameSize;
	Vector2 mapSize;
	wstring fileName;
	UINT tileCount;

	vector<InstanceData> instances;
	vector<Collider*> colliders;
	vector<Vector2> tilePos;

	vector<class Tile*> objTiles;

	VertexBuffer* instanceBuffer;

	Float4 noneColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Float4 downColor = { 0.0f, 0.0f, 0.7f, 1.0f };
	Float4 overColor = { 0.8f, 0.8f, 0.8f, 1.0f };

	UINT selectedTileNum = 0;
	wstring selectedObj;

	bool isObjMode = false;

	Vector2 clickPos;
};
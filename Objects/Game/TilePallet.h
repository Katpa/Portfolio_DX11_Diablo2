#pragma once

class TilePallet : public Transform
{
public:
	struct InstanceData
	{
		Matrix transform;
		Vector2 maxFrame;
		Vector2 curFrame;
		Float4 color;
	};

	struct ObjTile
	{
		Button* btn;
		wstring file;
	};

public:
	TilePallet(wstring file, Vector2 maxFrame);
	~TilePallet();

	void Update();
	void Render(bool isDebugMode = false);

	void ClickTile();

	void SetSelectedTileNum(UINT* variable) { selectedTileNum = variable; }
	void SetSelectedObj(wstring* obj) { selectedObj = obj; }

private:
	void SetPosition();
	void CreateObjTiles();

private:
	Quad* sprites;

	vector<InstanceData> instances;
	vector<Collider*> colliders;
	vector<Vector2> tilePos;
	vector<ObjTile> objTiles;

	Float4 noneColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Float4 downColor = { 0.0f, 0.0f, 0.7f, 1.0f };
	Float4 overColor = { 0.8f, 0.8f, 0.8f, 1.0f };

	UINT* selectedTileNum = 0;
	wstring* selectedObj;

	VertexBuffer* instanceBuffer;
};
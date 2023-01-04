#pragma once

class InstancingMap
{
public:
	InstancingMap(string file);
	~InstancingMap();

	void Update();
	void Render(bool isDebugMode);

	void GetNodes(vector<Node*>& nodes);

	UINT GetWidth() { return width; }
	Vector2 ReturnMapSize() { return mapSize; }
	
private:
	void Load(string file);

	void DrawObjTile();

private:
	Quad* sprites;

	vector<InstancingTileMap::InstanceData> instances;
	vector<Vector2> tilePos;

	VertexBuffer* instanceBuffer;

	vector<Tile*> objTiles;

	Vector2 mapSize;

	UINT width;

	RenderTarget* renderTarget;
};
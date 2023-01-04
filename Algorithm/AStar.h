#pragma once

class AStar //A*
{
public:
	AStar() {};
	AStar(class InstancingMap* map);
	~AStar();

	void Update();
	void Render();
		
	int FindCloseNode(Vector2 pos);

	void GetPath(IN int start,IN int end, OUT vector<Vector2>& path);
	void Position(float x, float y);

	Vector2 FindWarpPos(Vector2 pos);

protected:
	void Reset();

	float GetDistance(int start, int end);

	void Extend(int center, int end);
	int GetMinNode();	

	virtual void SetEdge(UINT width);

protected:
	vector<Node*> nodes;
	//vector<int> openNodes;
	Heap* heap;
};
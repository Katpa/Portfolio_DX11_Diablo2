#pragma once

class MiniMap
{
public:
	MiniMap(Vector2 mapSize);
	~MiniMap();

	void Update();
	void Render();

	void GetCharacter(Character* character) { player = character; }

private:
	Quad* miniMap;
	Quad* marker;

	Character* player;

	Vector2 mapSize;
	Vector2 mapHalfSize;
};
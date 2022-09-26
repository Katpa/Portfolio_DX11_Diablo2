#include "Framework.h"

GameTileMap::GameTileMap(string file)
{
	Load(file);

	tileSize = bgTiles.front()->Size();
	UINT mapWidth = (UINT)tileSize.x * width;
	UINT mapHeight = (UINT)tileSize.y * height;

	Vector2 mapSize(mapWidth, mapHeight);

	renderTarget = new RenderTarget(mapWidth, mapHeight);	
	targetTexture = new Quad(mapSize, mapSize * 0.5f);
	//타겟 부분 계산하기

	Texture* texture = Texture::Add(L"GameMap", renderTarget->GetSRV());
	targetTexture->SetTexture(texture);
	targetTexture->SetParent(this);

	DrawBGTile();
}

GameTileMap::~GameTileMap()
{
	for (Tile* tile : bgTiles)
		delete tile;

	for (Tile* tile : objTiles)
		delete tile;

	delete renderTarget;
	delete targetTexture;
}

void GameTileMap::Update(bool isDebugMode)
{
	targetTexture->UpdateWorld();
	
	UpdateWorld();

	for (Tile* tile : objTiles)
		tile->Update(isDebugMode);
}

void GameTileMap::Render(bool isDebugMode)
{
	targetTexture->Render();

	for (Tile* tile : objTiles)
		tile->Render(isDebugMode);
}

void GameTileMap::PushTank(Tank* tank)
{
	for (Tile* tile : objTiles)
	{
		if (tile->GetCollider()->IsCollision(tank->GetCollider()))
		{
			Direction dir = GetDirection(tile, tank);

			switch (dir)
			{
			case GameTileMap::UP:
				tank->Position().y += PUSH_SPEED * DELTA;
				break;
			case GameTileMap::DOWN:
				tank->Position().y -= PUSH_SPEED * DELTA;
				break;
			case GameTileMap::RIGHT:
				tank->Position().x += PUSH_SPEED * DELTA;
				break;
			case GameTileMap::LEFT:
				tank->Position().x -= PUSH_SPEED * DELTA;
				break;			
			}
		}
	}
}

void GameTileMap::GetNodes(vector<Node*>& nodes)
{
	for (Tile* tile : bgTiles)
	{
		tile->Update();

		Vector2 tilePos = tile->GlobalPosition();
		Node* node = new Node(tilePos, nodes.size());

		for (Tile* obj : objTiles)
		{
			Vector2 objPos = obj->GlobalPosition();
			if (objPos == tilePos)
			{
				node->SetState(Node::OBSTACLE);
			}
		}

		nodes.push_back(node);
	}
}

void GameTileMap::Load(string file)
{
	BinaryReader* reader = new BinaryReader(file);

	width = reader->UInt();
	height = reader->UInt();

	UINT size = reader->UInt();

	bgTiles.resize(size);

	for (Tile*& tile : bgTiles)
	{
		Tile::Data data;
		data.textureFile = reader->WString();
		data.pos = reader->Vector();
		data.type = (Tile::Type)reader->Int();

		tile = new Tile(data);	
		tile->Position() += tile->Size() * 0.5f;
		tile->SetParent(this);		
	}

	size = reader->UInt();

	objTiles.resize(size);

	for (Tile*& tile : objTiles)
	{
		Tile::Data data;
		data.textureFile = reader->WString();
		data.pos = reader->Vector();
		data.type = (Tile::Type)reader->Int();

		tile = new Tile(data);
		tile->Position() += tile->Size() * 0.5f;
		tile->SetParent(this);
	}

	delete reader;
}

void GameTileMap::DrawBGTile()
{
	renderTarget->Set();

	for (Tile* tile : bgTiles)
	{
		tile->UpdateWorld();
		tile->Render();
	}
}

GameTileMap::Direction GameTileMap::GetDirection(Transform* tile, Transform* target)
{
	Vector2 direction = target->GlobalPosition() - tile->GlobalPosition();

	if (direction.x > 0)
	{
		if (abs(direction.x) > abs(direction.y))
			return RIGHT;
		else
		{
			if (direction.y > 0)
				return UP;
			else
				return DOWN;
		}
	}
	else
	{
		if (abs(direction.x) > abs(direction.y))
			return LEFT;
		else
		{
			if (direction.y > 0)
				return UP;
			else
				return DOWN;
		}		
	}

	return NONE;
}

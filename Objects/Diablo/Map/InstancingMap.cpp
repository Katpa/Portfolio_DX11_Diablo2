#include "Framework.h"

InstancingMap::InstancingMap(string file)
{
    Load(file);

    CAM->SetLeftBottom(Vector2(0, 0));
    CAM->SetRightTop(mapSize);

    renderTarget = new RenderTarget(mapSize.x, mapSize.y);
    Texture* texture = Texture::Add(L"DiabloMap", renderTarget->GetSRV());

    renderTarget->Set();

    Render(true);
    DrawObjTile();

    for (Tile* tile : objTiles)
        ZOrder::Get()->Insert(tile);
}

InstancingMap::~InstancingMap()
{
    for (Tile* tile : objTiles)
        delete tile;
    objTiles.clear();

    delete instanceBuffer;
    delete renderTarget;
}

void InstancingMap::Update()
{
    for (Tile* tile : objTiles)
        tile->Update(false);
}

void InstancingMap::Render(bool isDebugMode)
{
    instanceBuffer->Set(1);
    sprites->SetRender();

    DC->DrawIndexedInstanced(6, instances.size(), 0, 0, 0);
    
    //DrawObjTile();
}

void InstancingMap::GetNodes(vector<Node*>& nodes)
{
	for (int i = 0; i < instances.size(); i++)
	{
		Node* node = new Node(tilePos[i], nodes.size());

		for (Tile* obj : objTiles)
		{
			Vector2 objPos = obj->Position();

			if (objPos == tilePos[i])
				node->SetState(Node::OBSTACLE);
		}

		nodes.push_back(node);
	}
}

void InstancingMap::Load(string file)
{
    BinaryReader* reader = new BinaryReader(file);

    mapSize = reader->Vector();
    wstring fileName = reader->WString();
    Vector2 maxFrame = reader->Vector();
    float MAP_MULTI_FACTOR = reader->Float();
    UINT tileCount = reader->UInt();
    width = sqrt(tileCount);

    sprites = new Quad(fileName, Vector2(), Vector2(1.0f / maxFrame.x, 1.0f / maxFrame.y));
    sprites->SetVertexShader(L"Shaders/VertexInstancing.hlsl");
    sprites->SetPixelShader(L"Shaders/PixelInstancing.hlsl");

    Texture* texture = Texture::Add(fileName);
    Vector2 frameSize = texture->GetSize() / maxFrame;

    instances.resize(tileCount);
    tilePos.resize(tileCount);

    for (int i = 0; i < tileCount; i++)
    {
        tilePos[i] = reader->Vector();

        Transform transform;

        transform.Position() = tilePos[i];
        transform.Scale() *= MAP_MULTI_FACTOR;

        transform.UpdateWorld();

        instances[i].transform = XMMatrixTranspose(transform.GetWorld());

        instances[i].maxFrame = maxFrame;
        instances[i].curFrame = reader->Vector();
        instances[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
    }

    instanceBuffer = new VertexBuffer(instances.data(), sizeof(InstancingTileMap::InstanceData), instances.size());

    objTiles.resize(reader->UInt());

    for (Tile*& tile : objTiles)
    {
        Tile::Data data;
        data.textureFile = reader->WString();
        data.pos = reader->Vector();
        data.type = Tile::OBJ;

        tile = new Tile(data);
        tile->Scale() *= MAP_MULTI_FACTOR;
    }
}

void InstancingMap::DrawObjTile()
{
    for (Tile* tile : objTiles)
        tile->Render();
}
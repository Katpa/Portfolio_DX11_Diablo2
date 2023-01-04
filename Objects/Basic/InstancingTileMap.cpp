#include "Framework.h"

InstancingTileMap::InstancingTileMap(wstring file, Vector2 maxFrame, UINT mapSize)
    :maxFrame(maxFrame), fileName(file)
{
    Texture* texture = Texture::Add(file);
    frameSize = texture->GetSize() / maxFrame;
    this->mapSize = frameSize * mapSize * MAP_MULTI_FACTOR;

    quad = new Quad(file, Vector2(), Vector2(1.0f / maxFrame.x, 1.0f / maxFrame.y));
    quad->SetVertexShader(L"Shaders/VertexInstancing.hlsl");
    quad->SetPixelShader(L"Shaders/PixelInstancing.hlsl");

    tileCount = mapSize * mapSize;
    instances.resize(tileCount);
    colliders.resize(tileCount);
    tilePos.resize(tileCount);

    Vector2 halfFrameSize = frameSize * 0.5f;
    Vector2 setTilePos = { halfFrameSize.x * mapSize * 1.5f, halfFrameSize.y * MAP_MULTI_FACTOR };

    for (int i = 0; i < instances.size(); i++)
    {
        if (i != 0)
        {
            if (i % mapSize == 0)
            {
                setTilePos.x -= halfFrameSize.x * mapSize * MAP_MULTI_FACTOR;
                setTilePos.y -= (halfFrameSize.y * (mapSize - 2)) * MAP_MULTI_FACTOR;
            }
            else
            {
                setTilePos += halfFrameSize * MAP_MULTI_FACTOR;
            }
        }

        Transform transform;
        
        transform.Position() = setTilePos;
        transform.Scale() *= MAP_MULTI_FACTOR;
        
        transform.UpdateWorld();

        instances[i].transform = XMMatrixTranspose(transform.GetWorld());

        instances[i].maxFrame = maxFrame;
        instances[i].curFrame.x = 0;
        instances[i].curFrame.y = 0;
        instances[i].color = noneColor;

        colliders[i] = new IsometricCollider(frameSize);
        colliders[i]->Position() = transform.Position();
        colliders[i]->Scale() *= MAP_MULTI_FACTOR;
        colliders[i]->UpdateWorld();

        tilePos[i] = setTilePos;
    }

    instanceBuffer = new VertexBuffer(instances.data(), sizeof(InstanceData), instances.size());
}

InstancingTileMap::~InstancingTileMap()
{
    for (Collider* collider : colliders)
        delete collider;
    colliders.clear();

    for (Tile* tile : objTiles)
        delete tile;
    objTiles.clear();

    delete quad;
    delete instanceBuffer;
}

void InstancingTileMap::Update()
{
    clickPos = CAM->ScreenToWorld(mousePos);

    ClickedTile();

    for (Tile* tile : objTiles)
        tile->Update();
}

void InstancingTileMap::Render(bool isDebugMode)
{
    instanceBuffer->Set(1);
    quad->SetRender();

    DC->DrawIndexedInstanced(6, instances.size(), 0, 0, 0);

    if (isDebugMode)
    {
        for (Collider* collider : colliders)
            collider->Render();
    }

    for (Tile* tile : objTiles)
        tile->Render();
}

void InstancingTileMap::ClickedTile()
{
    for (int i = 0; i < colliders.size(); i++)
    {
        instances[i].color = noneColor;
        if (colliders[i]->IsPointCollision(clickPos))
        {
            instances[i].color = overColor;
            if (KEY_PRESS(VK_LBUTTON))
            {
                if (KEY_PRESS(VK_LSHIFT))
                    SetObjTile(i);
                else
                    SetBgTile(i);

                instances[i].color = downColor;

                break;
            }
        }
    }

    if(KEY_DOWN(VK_RBUTTON))
        DeleteObject();

    instanceBuffer->Update(instances.data(), 0);
}

void InstancingTileMap::Save(string file)
{
    //맵크기               Vector2
    //텍스처 파일이름        wstring
    //맥스프레임            Vector2
    //맵배율               float
    //배경 타일
    // ->위치              Vector2
    // ->컬프레임 값        Vector2
    //바닥콜리전은 할 필요가 없다
    // 
    //오브젝트
    // ->파일명            wstring
    // ->위치             Vector2

    BinaryWriter* writer = new BinaryWriter(file);

    writer->Vector(mapSize);
    writer->WString(fileName);
    writer->Vector(maxFrame);
    writer->Float(MAP_MULTI_FACTOR);
    writer->UInt(tileCount);

    for (int i = 0; i < tileCount; i++)
    {
        writer->Vector(tilePos[i]);
        writer->Vector(instances[i].curFrame);
    }

    writer->UInt(objTiles.size());

    for (Tile* tile : objTiles)
    {
        writer->WString(tile->GetData().textureFile);
        writer->Vector(tile->GetData().pos);
    }
    delete writer;
}

void InstancingTileMap::Load(string file)
{
    DeleteAllTiles();

    BinaryReader* reader = new BinaryReader(file);

    mapSize = reader->Vector();
    fileName = reader->WString();
    maxFrame = reader->Vector();
    MAP_MULTI_FACTOR = reader->Float();
    tileCount = reader->UInt();

    quad = new Quad(fileName, Vector2(), Vector2(1.0f / maxFrame.x, 1.0f / maxFrame.y));
    quad->SetVertexShader(L"Shaders/VertexInstancing.hlsl");
    quad->SetPixelShader(L"Shaders/PixelInstancing.hlsl");

    Texture* texture = Texture::Add(fileName);
    frameSize = texture->GetSize() / maxFrame;

    instances.resize(tileCount);
    colliders.resize(tileCount);
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
        instances[i].color = noneColor;

        colliders[i] = new IsometricCollider(frameSize);
        colliders[i]->Position() = transform.Position();
        colliders[i]->Scale() *= MAP_MULTI_FACTOR;
        colliders[i]->UpdateWorld();
    }

    instanceBuffer = new VertexBuffer(instances.data(), sizeof(InstanceData), instances.size());

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

void InstancingTileMap::DeleteAllTiles()
{
    for (Collider* collider : colliders)
        delete collider;

    instances.clear();
    colliders.clear();
    tilePos.clear();

    for (Tile* tile : objTiles)
        delete tile;
    objTiles.clear();

    delete instanceBuffer;
    delete quad;
}

void InstancingTileMap::DeleteObject()
{
    if (objTiles.size() < 1)
        return;

    for (int i = 0; i < objTiles.size(); i++)
    {
        if (objTiles[i]->GetCollider()->IsPointCollision(clickPos))
        {
            objTiles.erase(objTiles.begin() + i);
        }
    }
}

void InstancingTileMap::SetBgTile(UINT index)
{
    instances[index].curFrame.x = selectedTileNum;
}

void InstancingTileMap::SetObjTile(UINT index)
{
    DeleteObject();

    Tile::Data data;
    data.textureFile = selectedObj;
    data.type = Tile::OBJ;
    data.pos = tilePos[index];

    Tile* tile = new Tile(data);
    tile->Scale() *= MAP_MULTI_FACTOR;

    objTiles.push_back(tile);
}
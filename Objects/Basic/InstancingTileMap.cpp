#include "Framework.h"

InstancingTileMap::InstancingTileMap(wstring file, Vector2 maxFrame, UINT amount, UINT width, UINT height)
    :maxFrame(maxFrame), type(RECT)
{
    Texture* texture = Texture::Add(file);
    frameSize = texture->GetSize() / maxFrame;
    this->mapSize = Vector2(width, height);

    quad = new Quad(file, Vector2(), Vector2(1.0f / maxFrame.x, 1.0f / maxFrame.y));
    quad->SetVertexShader(L"Shaders/VertexInstancing.hlsl");
    quad->SetPixelShader(L"Shaders/PixelInstancing.hlsl");

    instances.resize(amount);
    colliders.resize(amount);
    states.resize(amount);

    for (int i = 0; i < instances.size(); i++)
    {
        Transform transform;

        transform.Position().x = 0;
        transform.Position().y = 0;

        transform.UpdateWorld();

        instances[i].transform = XMMatrixTranspose(transform.GetWorld());

        instances[i].maxFrame = maxFrame;
        instances[i].curFrame.x = 0;
        instances[i].curFrame.y = 0;
        instances[i].color = noneColor;

        colliders[i] = new RectCollider(frameSize);
        colliders[i]->Position() = transform.Position();

        states[i] = NONE;
    }

    instanceBuffer = new VertexBuffer(instances.data(), sizeof(InstanceData), instances.size());
}

InstancingTileMap::InstancingTileMap(wstring file, Vector2 maxFrame, UINT mapSize)
    :maxFrame(maxFrame), type(ISOMETRIC)
{
    Texture* texture = Texture::Add(file);
    frameSize = texture->GetSize() / maxFrame;
    this->mapSize = Vector2(mapSize, mapSize);

    quad = new Quad(file, Vector2(), Vector2(1.0f / maxFrame.x, 1.0f / maxFrame.y));
    quad->SetVertexShader(L"Shaders/VertexInstancing.hlsl");
    quad->SetPixelShader(L"Shaders/PixelInstancing.hlsl");

    UINT amount = mapSize * mapSize;
    instances.resize(amount);
    colliders.resize(amount);
    states.resize(amount);

    //�׷��ϱ� �ʱ���ġ�� �� ����ְ� �� �������� �ؼ� ���� �а� �������� ��ġ??
    //���� ���̼� ��Ʈ���̰� �׸��� ���̽�Ÿ�� ����ؼ� ������ �ϴϱ�
    //������ ��Ҵ� �� ������� �а� �����ڱ� ����?

    Vector2 halfFrameSize = frameSize * 0.5f;
    Vector2 setTilePos = { halfFrameSize.x * mapSize, halfFrameSize.y };

    for (int i = 0; i < instances.size(); i++)
    {
        Transform transform;

        transform.Position() = setTilePos;

        transform.UpdateWorld();

        instances[i].transform = XMMatrixTranspose(transform.GetWorld());

        instances[i].maxFrame = maxFrame;
        instances[i].curFrame.x = 0;
        instances[i].curFrame.y = 0;
        instances[i].color = noneColor;

        colliders[i] = new RectCollider(frameSize);
        colliders[i]->Position() = transform.Position();

        states[i] = NONE;
    }

    instanceBuffer = new VertexBuffer(instances.data(), sizeof(InstanceData), instances.size());
}

InstancingTileMap::~InstancingTileMap()
{
    for (Collider* collider : colliders)
        delete collider;
    delete quad;
    delete instanceBuffer;
}

void InstancingTileMap::Update()
{
}

void InstancingTileMap::Render(bool isDebugMode)
{
    instanceBuffer->Set(1);
    quad->SetRender();

    DC->DrawIndexedInstanced(6, instances.size(), 0, 0, 0);
}

void InstancingTileMap::ClickedTile()
{
}

void InstancingTileMap::SetUp()
{
}

void InstancingTileMap::SetDown()
{
}

void InstancingTileMap::Save(string file)
{
}

void InstancingTileMap::Load(string file)
{
}

void InstancingTileMap::CreateTile()
{
}

void InstancingTileMap::SetTexture(UINT frameNum)
{
}

UINT InstancingTileMap::GetTexture(Vector2 pos)
{
    return 0;
}

void InstancingTileMap::MoveMapPos(Vector2 movePos)
{
}

void InstancingTileMap::InitMapPos()
{
}

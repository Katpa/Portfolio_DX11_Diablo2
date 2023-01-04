#include "Framework.h"

TilePallet::TilePallet(wstring file, Vector2 maxFrame)
{
    Position() = {};
    UpdateWorld();
    this->SetParent(CAM);

    Texture* texture = Texture::Add(file);
    Vector2 frameSize = texture->GetSize() / maxFrame;

    sprites = new Quad(file, Vector2(), Vector2(1.0f / maxFrame.x, 1.0f / maxFrame.y));
    sprites->SetVertexShader(L"Shaders/VertexInstancing.hlsl");
    sprites->SetPixelShader(L"Shaders/PixelInstancing.hlsl");

    UINT amount = maxFrame.x * maxFrame.y;
    instances.resize(amount);
    colliders.resize(amount);
    this->tilePos.resize(amount);

    //초기 위치
    Vector2 tilePos = { WIN_WIDTH - frameSize.x * 1.5f, WIN_HEIGHT + frameSize.y * 0.5f };

    for (int i = 0; i < instances.size(); i++)
    {
        if (tilePos.y < frameSize.y)
        {
            tilePos = Vector2(tilePos.x + frameSize.x, WIN_HEIGHT - frameSize.y * 0.5f);
        }
        else
        {
            tilePos.y -= frameSize.y;
        }

        Transform transform;

        transform.Position() = tilePos;

        transform.UpdateWorld();

        instances[i].transform = XMMatrixTranspose(transform.GetWorld());

        instances[i].maxFrame = maxFrame;
        instances[i].curFrame.x = i;
        instances[i].curFrame.y = 0;
        instances[i].color = noneColor;

        colliders[i] = new RectCollider(frameSize);
        colliders[i]->Position() = transform.Position();
        colliders[i]->UpdateWorld();

        this->tilePos[i] = transform.Position();
    }

    instanceBuffer = new VertexBuffer(instances.data(), sizeof(InstanceData), instances.size());

    CreateObjTiles();
}

TilePallet::~TilePallet()
{
    for (Collider* collider : colliders)
        delete collider;

    for (ObjTile& tile : objTiles)
        delete tile.btn;

    delete sprites;
    delete instanceBuffer;
}

void TilePallet::Update()
{
    ClickTile();

    for (ObjTile& tile : objTiles)
        tile.btn->Update();

    UpdateWorld();
    SetPosition();
}

void TilePallet::Render(bool isDebugMode)
{
    instanceBuffer->Set(1);
    sprites->SetRender();

    DC->DrawIndexedInstanced(6, instances.size(), 0, 0, 0);

    for (ObjTile tile : objTiles)
        tile.btn->Render();

    if (isDebugMode)
    {
        for (Collider* collider : colliders)
            collider->Render();
    }
}

void TilePallet::ClickTile()
{
    Vector2 clickPos = CAM->ScreenToWorld(mousePos);

    for (int i = 0; i < colliders.size(); i++)
    {
        instances[i].color = noneColor;
        if (colliders[i]->IsPointCollision(clickPos))
        {
            instances[i].color = overColor;
            if (KEY_PRESS(VK_LBUTTON))
            {
                *selectedTileNum = instances[i].curFrame.x;
                instances[i].color = downColor;
                break;
            }
        }
    }
    instanceBuffer->Update(instances.data(), 0);

    for (ObjTile& tile : objTiles)
    {
        if (tile.btn->ReturnBtnState() == 1)
            *selectedObj = tile.btn->ReturnFileName();
    }
}

void TilePallet::SetPosition()
{
    for (int i = 0; i < instances.size(); i++)
    {
        Vector2 tmpPos = this->GlobalPosition() + tilePos[i];
        Transform transform;

        transform.Position() = tmpPos;

        transform.UpdateWorld();

        instances[i].transform = XMMatrixTranspose(transform.GetWorld());

        colliders[i]->Position() = tmpPos;
        colliders[i]->UpdateWorld();
    }

    instanceBuffer->Update(instances.data(), 0);
}

void TilePallet::CreateObjTiles()
{
    {
        wstring fileDirectory = L"Textures/Tile/object/";
        wstring filePrefix = L"obj";
        wstring fileNumber;

        Vector2 size(72, 72);
        Vector2 btnPos = { size.x * 0.5f, WIN_HEIGHT - size.y * 0.5f };

        for(int i = 1; i < 16; i++)
        {
            string tmpStr;

            if (i > 9)
                tmpStr = to_string(i);
            else
                tmpStr = "0" + to_string(i);

            fileNumber.assign(tmpStr.begin(), tmpStr.end());

            wstring file = fileDirectory + filePrefix + fileNumber + L".png";

            ObjTile tile;
            tile.btn = new Button(Vector2(72, 72), file);
            tile.btn->Position() = btnPos;
            tile.btn->SetParent(this);

            tile.file = file;

            objTiles.push_back(tile);

            btnPos.y -= size.y;
            if (btnPos.y < 1)
            {
                btnPos.x += size.x;
                btnPos.y = WIN_HEIGHT - size.y * 0.5f;
            }
        }
    }
}
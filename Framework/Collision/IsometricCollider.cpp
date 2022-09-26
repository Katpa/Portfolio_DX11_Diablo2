#include "Framework.h"

IsometricCollider::IsometricCollider(Vector2 size) : size(size)
{
    type = Type::ISOMETRIC;

    CreateLine();
}

bool IsometricCollider::IsPointCollision(Vector2 point)
{
    Matrix invWorld = XMMatrixInverse(nullptr, world);
    Vector2 tempPos = point * invWorld;

    Vector2 halfSize = size * 0.5f;

    if (halfSize.x > fabs(tempPos.x) &&
        halfSize.y > fabs(tempPos.y))
    {
        if ((1 - (fabs(tempPos.x) / halfSize.x)) * halfSize.y > fabs(tempPos.y))
            return true;
    }

    return false;
}

bool IsometricCollider::IsRectCollision(RectCollider* rect, Vector2* overlap)
{
    return false;
}

bool IsometricCollider::IsCircleCollision(CircleCollider* circle)
{
    return false;
}

void IsometricCollider::CreateLine()
{
    Vector2 half = size * 0.5f;

    vertices.emplace_back(-half.x, 0);
    vertices.emplace_back(0, +half.y);
    vertices.emplace_back(+half.x, 0);
    vertices.emplace_back(0, -half.y);
    vertices.emplace_back(-half.x, 0);

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexPos), vertices.size());
}

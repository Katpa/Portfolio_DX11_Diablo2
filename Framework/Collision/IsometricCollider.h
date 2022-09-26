#pragma once

class IsometricCollider : public Collider
{
public:
	IsometricCollider(Vector2 size);

	virtual bool IsPointCollision(Vector2 point) override;
	virtual bool IsRectCollision(RectCollider* rect, Vector2* overlap = nullptr) override;
	virtual bool IsCircleCollision(CircleCollider* circle) override;

private:
	void CreateLine();

private:
	Vector2 size;
};
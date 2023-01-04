#pragma once

class ImageFont : public Transform
{
private:
	const UINT LENGTH = 8;

public:
	ImageFont(vector<Texture*> images,
		Vector2 size = Vector2(50, 50), float interval = 5.0f);
	~ImageFont();
	
	void Update();
	void Render();

	void SetValue(UINT value) { this->value = value; }

private:
	UINT value = 0;

	vector<Quad*> quads;
	vector<Texture*> images;

	float interval;
};
#include "Framework.h"

ImageFont::ImageFont(vector<Texture*> images, Vector2 size, float interval)
	: images(images), interval(interval)
{
	quads.resize(LENGTH);

	for (UINT i = 0; i < LENGTH; i++)
	{
		quads[i] = new Quad(size);
		quads[i]->Position().x = i * -size.x - interval * i;
		quads[i]->Active() = false;
		quads[i]->SetParent(this);
	}
}

ImageFont::~ImageFont()
{
	for (Quad* quad : quads)
		delete quad;
}

void ImageFont::Update()
{
	if (!isActive) return;

	string str = to_string(value);

	for (UINT i = 0; i < str.size(); i++)
	{
		int num = str[i] - '0';
		int quadIndex = str.size() - i - 1;
		quads[quadIndex]->SetTexture(images[num]);
		quads[quadIndex]->Active() = true;
		quads[quadIndex]->UpdateWorld();
	}

	for (UINT i = str.size(); i < quads.size(); i++)
	{
		quads[i]->Active() = false;
	}

	UpdateWorld();
}

void ImageFont::Render()
{
	if (!isActive) return;

	for (Quad* quad : quads)
		quad->Render();
}

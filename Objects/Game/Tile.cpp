#include "Framework.h"

Tile::Tile(Data data)
	: IButton(data.textureFile), data(data)
{
	localPosition = data.pos;

	if (data.type == BG)
		collider->Active() = true;
}

Tile::~Tile()
{
}

void Tile::SetTile(wstring file)
{
	SetTexture(Texture::Add(file));
	data.textureFile = file;
}

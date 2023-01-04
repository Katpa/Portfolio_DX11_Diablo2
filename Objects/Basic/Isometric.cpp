#include "Framework.h"

Isometric::Isometric(wstring file, Vector2 startUV, Vector2 endUV, Vector2 pos)
{
	texture = Texture::Add(file);

	Vector2 size = (endUV - startUV) * texture->GetSize();
	float halfUV = 0.5f;

	float left = pos.x - size.x * 0.5f;
	float right = pos.x + size.x * 0.5f;
	float top = pos.y + size.y * 0.5f;
	float bottom = pos.y - size.y * 0.5f;

	//vertices.emplace_back(pos.x, top, startUV.x + halfUV, startUV.y);
	//vertices.emplace_back(right, pos.y, endUV.x, startUV.y + halfUV);
	//vertices.emplace_back(left, pos.y, startUV.x, endUV.y - halfUV);
	//vertices.emplace_back(pos.x, bottom, startUV.x + halfUV, endUV.y);

	vertices.emplace_back(left, top, startUV.x, startUV.y);
	vertices.emplace_back(right, top, endUV.x, startUV.y);
	vertices.emplace_back(left, bottom, startUV.x, endUV.y);
	vertices.emplace_back(right, bottom, endUV.x, endUV.y);

	indices = { 0, 1, 2, 2, 1, 3 };

	Init();
}

Isometric::Isometric(Vector2 size, Vector2 pos)
{
	float left = pos.x - size.x * 0.5f;
	float right = pos.x + size.x * 0.5f;
	float top = pos.y + size.y * 0.5f;
	float bottom = pos.y - size.y * 0.5f;

	vertices.emplace_back(pos.x, top, 0.5f, 0);
	vertices.emplace_back(right, pos.y, 1, 0.5f);
	vertices.emplace_back(left, pos.y, 0, 0.5f);
	vertices.emplace_back(pos.x, bottom, 0.5f, 1);

	indices = { 0, 1, 2, 2, 1, 3 };

	Init();
}

Isometric::~Isometric()
{
	delete vertexBuffer;
	delete indexBuffer;

	delete worldBuffer;
	delete colorBuffer;
}

void Isometric::Render()
{
	if (this == nullptr) return;

	worldBuffer->Set(world);
	worldBuffer->SetVS(0);
	colorBuffer->SetPS(0);

	if (texture)
		texture->PSSet();

	vertexBuffer->Set();
	indexBuffer->Set();

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Isometric::SetPixelShader(wstring file)
{
	pixelShader = Shader::AddPS(file);
}

void Isometric::Init()
{
	vertexShader = Shader::AddVS(L"Shaders/VertexUV.hlsl");
	pixelShader = Shader::AddPS(L"Shaders/PixelUV.hlsl");

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	worldBuffer = new MatrixBuffer();
	colorBuffer = new ColorBuffer();
}

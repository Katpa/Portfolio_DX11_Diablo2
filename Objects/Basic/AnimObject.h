#pragma once

class AnimObject : public GameObject
{
public:
	AnimObject();
	~AnimObject();

	void Update();
	void Render();

protected:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	MatrixBuffer* worldBuffer;
	ColorBuffer* colorBuffer;

	map<int, Clip*> clips;
	Clip* curClip;
};

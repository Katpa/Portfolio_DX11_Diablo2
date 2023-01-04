#pragma once

class AnimObject : public GameObject
{
public:
	AnimObject();
	~AnimObject();

	void Update();
	virtual void Render();

	void SetPixelShader(wstring file);
	ColorBuffer* GetColorBuffer() { return colorBuffer; }

protected:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	MatrixBuffer* worldBuffer;
	ColorBuffer* colorBuffer;

	map<int, Clip*> clips;
	Clip* curClip;
};

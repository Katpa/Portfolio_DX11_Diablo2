#pragma once

class TextureScene : public Scene
{
public:
	TextureScene();
	~TextureScene();

	virtual void Update() override;
	virtual void Render() override;

private:
	Quad* quad;

	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* samplerState;

	float speed = 100.0f;

	Float4 color;
};
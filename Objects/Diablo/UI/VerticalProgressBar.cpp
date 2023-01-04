#include "Framework.h"

VerticalProgressBar::VerticalProgressBar(wstring file, int* curValue, UINT* maxValue)
	:Quad(file), curValue(curValue), maxValue(maxValue)
{
	SetPixelShader(L"Shaders/PixelVerticalBar.hlsl");

	valueBuffer = new FloatValueBuffer();
}

VerticalProgressBar::~VerticalProgressBar()
{
	delete valueBuffer;
}

void VerticalProgressBar::Render()
{
	valueBuffer->SetPS(1);

	Quad::Render();
}

void VerticalProgressBar::Update()
{
	SetValue();

	UpdateWorld();
}

void VerticalProgressBar::SetValue()
{
	valueBuffer->data.values[0] = 1 - (float)*curValue / (float)*maxValue;
}
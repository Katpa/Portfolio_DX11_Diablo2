#include "Framework.h"

HorizentalProgressBar::HorizentalProgressBar(wstring file, int* curValue, UINT* maxValue)
	:Quad(file), curIntValue(curValue), maxValue(maxValue)
{
	SetPixelShader(L"Shaders/PixelHorizentalBar.hlsl");

	valueBuffer = new FloatValueBuffer();

	curFloatValue = nullptr;
}

HorizentalProgressBar::HorizentalProgressBar(wstring file, float* curValue, UINT* maxValue)
	:Quad(file), curFloatValue(curValue), maxValue(maxValue)
{
	SetPixelShader(L"Shaders/PixelHorizentalBar.hlsl");

	valueBuffer = new FloatValueBuffer();

	curIntValue = nullptr;
}

HorizentalProgressBar::~HorizentalProgressBar()
{
	delete valueBuffer;
}

void HorizentalProgressBar::Render()
{
	valueBuffer->SetPS(1);

	Quad::Render();
}

void HorizentalProgressBar::Update()
{
	SetValue();

	UpdateWorld();
}

void HorizentalProgressBar::SetValue()
{
	if(curIntValue != nullptr)
		valueBuffer->data.values[0] = (float)*curIntValue / (float)*maxValue;
	else
		valueBuffer->data.values[0] = *curFloatValue / (float)*maxValue;
}

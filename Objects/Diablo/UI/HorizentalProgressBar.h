#pragma once

class HorizentalProgressBar : public Quad
{
public:
	HorizentalProgressBar(wstring file, int* curValue, UINT* maxValue);
	HorizentalProgressBar(wstring file, float* curValue, UINT* maxValue);
	~HorizentalProgressBar();

	void Render();
	void Update();

private:
	void SetValue();

private:
	FloatValueBuffer* valueBuffer;
	int* curIntValue;
	float* curFloatValue;
	UINT* maxValue;
};
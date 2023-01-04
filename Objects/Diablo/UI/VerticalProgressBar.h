#pragma once

class VerticalProgressBar : public Quad
{
public:
	VerticalProgressBar(wstring file, int* curValue, UINT* maxValue);
	~VerticalProgressBar();

	void Render();
	void Update();

private:
	void SetValue();

private:
	FloatValueBuffer* valueBuffer;
	int* curValue;
	UINT* maxValue;
};
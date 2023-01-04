#include "Framework.h"

Font::Font()
{
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory), (IUnknown**)&writeFactory);

	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);

	IDXGIDevice* dxgiDevice;
	DEVICE->QueryInterface(&dxgiDevice);

	factory->CreateDevice(dxgiDevice, &device);

	device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		&context);

	IDXGISurface* dxgiSurface;
	Device::Get()->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiSurface);

	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96;
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = nullptr;

	context->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &targetBitmap);

	context->SetTarget(targetBitmap);
}

Font::~Font()
{
	for (pair<string, FontDesc*> font : fonts)
		delete font.second;

	factory->Release();
	writeFactory->Release();

	targetBitmap->Release();

	context->Release();
	device->Release();
}

void Font::Add(string key, wstring font, Float3 color, float size, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
	if (fonts.count(key) > 0) return;

	FontDesc* desc = new FontDesc();

	D2D1::ColorF colorF(color.x, color.y, color.z);
	context->CreateSolidColorBrush(colorF, &desc->brush);

	writeFactory->CreateTextFormat(font.c_str(), nullptr,
		weight, style, stretch, size, L"ko", &desc->format);

	desc->format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	desc->format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	fonts[key] = desc;
}

void Font::RenderText(wstring text, string key, Vector2 pos, Vector2 size)
{
	if (fonts.count(key) == 0) return;

	if (size == Vector2())
	{
		size.x = text.size() * fonts[key]->format->GetFontSize();
		size.y = fonts[key]->format->GetFontSize();
	}

	Vector2 halfSize = size * 0.5f;

	pos.y = WIN_HEIGHT - pos.y;

	D2D1_RECT_F rectF;
	rectF.left = pos.x - halfSize.x;
	rectF.right = pos.x + halfSize.x;
	rectF.top = pos.y - halfSize.y;
	rectF.bottom = pos.y + halfSize.y;

	context->DrawTextW(text.c_str(), text.size(),
		fonts[key]->format, &rectF, fonts[key]->brush);
}

void Font::RenderText(string text, string key, Vector2 pos, Vector2 size)
{
	if (fonts.count(key) == 0) return;

	if (size == Vector2())
	{
		size.x = text.size() * fonts[key]->format->GetFontSize();
		size.y = fonts[key]->format->GetFontSize();
	}

	Vector2 halfSize = size * 0.5f;

	pos.y = WIN_HEIGHT - pos.y;

	D2D1_RECT_F rectF;
	rectF.left = pos.x - halfSize.x;
	rectF.right = pos.x + halfSize.x;
	rectF.top = pos.y - halfSize.y;
	rectF.bottom = pos.y + halfSize.y;

	wstring temp = ChangeWString(text);

	context->DrawTextW(temp.c_str(), temp.size(),
		fonts[key]->format, &rectF, fonts[key]->brush);
}

wstring Font::ChangeWString(string value)
{
	int nLen = MultiByteToWideChar(CP_ACP, 0, &value[0], value.size(), nullptr, 0);
	wstring strUni(nLen + 1, 0);
	int len = MultiByteToWideChar(CP_ACP, 0, value.c_str(), value.size(), &strUni[0], nLen);
	strUni[len] = 0;

	return strUni;
}
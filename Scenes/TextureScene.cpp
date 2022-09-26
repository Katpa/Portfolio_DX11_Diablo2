#include "Framework.h"
#include "TextureScene.h"

TextureScene::TextureScene()
{
	quad = new Quad(Vector2(200, 200), { 100, 100 });
	quad->Position() = { CENTER_X, CENTER_Y };
	quad->SetPivot(0, 100);

	ScratchImage image;
	LoadFromWICFile(L"Textures/airplane.png",
		WIC_FLAGS_NONE, nullptr, image);

	CreateShaderResourceView(DEVICE, image.GetImages(),
		image.GetImageCount(), image.GetMetadata(), &srv);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;	
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//LOD(Level Of Detail) : 카메라의 거리에 따라서 
	//퀄리티를 다르게 표현하는 방법

	DEVICE->CreateSamplerState(&samplerDesc, &samplerState);
}

TextureScene::~TextureScene()
{
	delete quad;
	srv->Release();
	samplerState->Release();
}

void TextureScene::Update()
{
	//if (KEY_PRESS('W'))
	//	quad->Position().y += speed * DELTA;
	//if (KEY_PRESS('S'))
	//	quad->Position().y -= speed * DELTA;
	//if (KEY_PRESS('A'))
	//	quad->Position().x -= speed * DELTA;
	//if (KEY_PRESS('D'))
	//	quad->Position().x += speed * DELTA;
	//
	//Vector2 direction = mousePos - quad->Position();
	//
	//float angle = atan2(direction.y, direction.x);
	//
	//quad->Rotation().z = angle;	

	quad->UpdateWorld();
}

void TextureScene::Render()
{
	DC->PSSetShaderResources(0, 1, &srv);
	DC->PSSetSamplers(0, 1, &samplerState);

	quad->Render();

	ImGui::DragFloat2("Pos", (float*)&quad->Position());
	ImGui::DragFloat3("Rot", (float*)&quad->Rotation(), 0.01f);
	ImGui::DragFloat2("Scale", (float*)&quad->Scale());	

	ImGui::ColorEdit4("Color", (float*)&color);

	quad->GetColorBuffer()->Set(color);
}

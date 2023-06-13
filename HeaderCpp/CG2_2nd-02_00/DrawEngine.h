#pragma once
#include"DirectX.h"
class DrawEngine
{
public:
	void Initialize(DirectX* directX);

	void Draw(Vector4 Leftbottom,Vector4 top,Vector4 Rightbottom);

	void Release();

private:
	HRESULT hr;
	DirectX* directX_=nullptr;
	//バーテックスリソース
	ID3D12Resource* vertexResource = nullptr;
	//バーテックスバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	Vector4* vertexData=nullptr;
	void MakeVertexResource();
	void MakeVertexBufferView();
};

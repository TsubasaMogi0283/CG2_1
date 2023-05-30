#pragma once
#include"DirectX.h"
class DrawEngine
{
public:
	void Initialize(DirectX* directX);

	void Draw(Vector4 Rightbottom, Vector4 top, Vector4 Leftbottom);

	void Release();

private:
	HRESULT hr;
	ID3D12GraphicsCommandList* commandList_=nullptr;
	ID3D12Device* device_ = nullptr;
	//バーテックスリソース
	ID3D12Resource* vertexResource = nullptr;
	//バーテックスバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	void MakeVertexResource();
	void MakeVertexBufferView();
};

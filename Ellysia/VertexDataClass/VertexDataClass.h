#pragma once
#include "Common/DirectX/DirectXSetup.h"
#include <Math/Vector/VertexData.h>
#include <vector>

class VertexDataClass{
public:

	void Initialize(const std::vector<VertexData>& vertices);

	void SetGraphicCommand();

	void DrawCall();

private:
	//Resource作成の関数化
	static ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

private:

	//Lighting用
	ComPtr<ID3D12Resource> resource_ = nullptr;
	VertexData* data_ = nullptr;
	//頂点データ
	std::vector<VertexData> vertices_{};
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
};


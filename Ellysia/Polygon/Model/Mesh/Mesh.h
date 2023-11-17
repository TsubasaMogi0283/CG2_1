#pragma once

#include <vector>
#include <Math/Vector/VertexData.h>
#include "Common/DirectX/DirectXSetup.h"

class Mesh{
public:

	void Initialize(const std::vector<VertexData>& vertices);

	void SetGraphicsCommand();

	void DrawCall();


private:
	//頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
		

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	std::vector<VertexData> vertices_{};

};


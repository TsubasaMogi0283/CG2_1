#pragma once
#include "Common/DirectX/DirectXSetup.h"
#include <Math/Vector/Material.h>

class MaterialDataClass {
public:

	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	void Initialize();

	void DrawInformation(Vector4 color);

	void SetGraphicCommand();

private:

	ComPtr<ID3D12Resource> resource_ = nullptr;
	Material* data_ = nullptr;
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

};


#pragma once
#include "Common/DirectX/DirectXSetup.h"
#include <Math/Vector/DirectionalLight.h>

class CreateDirectionalLight{
public:
	//Resource作成の関数化
	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	void Initialize();


	void SetGraphicsCommand();

	



private:
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;




};


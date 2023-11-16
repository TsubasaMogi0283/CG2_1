#pragma once
#include "Common/DirectX/DirectXSetup.h"
#include <Math/Vector/DirectionalLight.h>

class CreateDirectionalLight{
public:

	void Initialize();


	void SetGraphicsCommand();

	



private:
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;




};


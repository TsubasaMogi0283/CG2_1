#pragma once
#include "Common/DirectX/DirectXSetup.h"
#include "Math/Vector/Transform.h"

class Instancing{
public:
	void Initialize();

	void SetGraphicsCommand();

	void DrawCall();


private:
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU = {};

	Transform transforms[instanceCount_];
};


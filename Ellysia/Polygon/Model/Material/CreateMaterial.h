#pragma once
#include "Math/Vector/Material.h"
#include "Common/DirectX/DirectXSetup.h"
#include "Math/Vector/Vector4.h"

class CreateMaterial{
public:
	//Resource作成の関数化
	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	void Initialize();

	void SetInformation(Vector4 setColor);

	void SetGraphicsCommand();

	


private:
	//頂点リソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
		

	Material* materialData_ = nullptr;


};


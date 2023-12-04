#pragma once
#include "Math/Vector/Material.h"
#include "Common/DirectX/DirectXSetup.h"
#include "Math/Vector/Vector4.h"

class CreateMaterial{
public:

	void Initialize();

	void SetInformation(Vector4 setColor);

	void GraphicsCommand();

	
	struct Material {
	Vector4 color;
	//boolの代わりにint32_t
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

private:
	//頂点リソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
		

	


};


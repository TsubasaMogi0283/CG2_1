#pragma once
#include "Common/DirectX/DirectXSetup.h"
#include <Math/Matrix/Matrix/TransformationMatrix.h>
#include <Math/Vector/Transform.h>

class TransformationDataClass{
public:

	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	void Initialize();

	void SetDrawInformation(Transform transform);

	void SetGraphicCommand();


private:
	ComPtr<ID3D12Resource> resource_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;


};


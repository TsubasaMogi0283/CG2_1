#pragma once
#include "Common/DirectX/DirectXSetup.h"

#include <Math/Vector/Transform.h>
#include <Math/Matrix/Matrix/TransformationMatrix.h>

class Transformation{
public:

	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	void Initialize();

	void SetInformation(Transform transform);

	void SetGraphicCommand();

private:

	ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;
};


#pragma once
#include "DirectXSetup.h"

#include <Transform.h>
#include <Math/Matrix/Matrix/TransformationMatrix.h>

class Transformation{
public:


	void Initialize();

	void SetInformation(Transform transform);

	void SetGraphicCommand();

private:

	ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;
};


#pragma once
#include "Vector4.h"
#include <cstdint>
#include <Math/Matrix/Matrix/Matrix3x3.h>

struct Material {
	Vector4 color;
	//boolの代わりにint32_t
	int32_t enableLighting;
	Matrix3x3 uvTransform;
};
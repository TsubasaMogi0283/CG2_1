#pragma once
#include "Math/Matrix/Matrix/Matrix4x4.h"

struct WorldViewMatrix {
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
};

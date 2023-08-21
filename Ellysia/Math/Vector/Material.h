#pragma once
#include "Vector4.h"
#include <cstdint>

struct Material {
	Vector4 color;
	//boolの代わりにint32_t
	int32_t enableLighting;
};
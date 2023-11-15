#pragma once
#include <vector>
#include <Math/Vector/VertexData.h>
#include "Polygon/Model/MaterialData/MaterialData.h"


struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	std::string name;
};
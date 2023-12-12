#pragma once
#include <vector>
#include <VertexData.h>
#include "Polygon/3D/Model/MaterialData/MaterialData.h"


struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	std::string name;
};
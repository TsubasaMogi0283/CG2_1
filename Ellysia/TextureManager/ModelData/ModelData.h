#pragma once
#include <vector>
#include <Math/Vector/VertexData.h>
#include <TextureManager/MaterialData/MaterialData.h>


struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	int textureIndex;
};
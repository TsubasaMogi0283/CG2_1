#pragma once
#include "Math/Vector/ModelData.h"
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>

static ModelData LoadObjFile(const std::string& directoryPath, const std::string& fileName);
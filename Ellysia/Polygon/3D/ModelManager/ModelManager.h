#pragma once
#include <string>
#include <ModelData.h>
#include <fstream>
#include <sstream>

//モデルデータの読み込み
ModelData LoadObjectFile(const std::string& directoryPath, const std::string& fileName);

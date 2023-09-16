#pragma once
#include "Math/Vector/ModelData.h"
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>

ModelData LoadObjFile(const std::string& directoryPath, const std::string& fileName) {
	//1.中で必要となる変数の宣言
	//構築するModelData
	ModelData modelData;
	//位置
	std::vector<Vector4> position;
	//法線
	std::vector<Vector3> normal;
	//テクスチャ座標
	std::vector<Vector2> texcoords;
	//ファイルから読んだ1行を格納するもの
	std::string line;


	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + fileName);
	assert(file.is_open());
	 
	//3.実際にファイルを読み、ModelDataを構築していく
	//4.ModelDataを返す
}

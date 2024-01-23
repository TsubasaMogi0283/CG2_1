#pragma once
#include "Vector4.h"
#include <cstdint>
#include "Matrix4x4.h"



struct Material {
	Vector4 color;
	//boolの代わりにint32_t
	int32_t enableLighting;
	//穴埋め用
	//詳しくはCG2_05_01で
	float padding[3];
	//UVTransform
	Matrix4x4 uvTransform;
	//光沢度
	float shininess;
};



//struct Material {
//	Vector4 color;
//	//boolの代わりにint32_t
//	int32_t enableLighting;
//	//穴埋め用
//	float padding[3];
//	Matrix4x4 uvTransform;
//};

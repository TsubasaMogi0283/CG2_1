#pragma once
#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Vector/Vector3.h>

struct WorldTransform {


	//スケール
	Vector3 scale_ = {1.0f, 1.0f, 1.0f};
	//回転角
	Vector3 rotation_ = {0.0f, 0.0f, 0.0f};
	//座標
	Vector3 translation_ = {0.0f, 0.0f, 0.0f};
	//ワールド変換行列
	Matrix4x4 matWorld_{};
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;


	//行列の計算
	void UpdateMatrix();



};


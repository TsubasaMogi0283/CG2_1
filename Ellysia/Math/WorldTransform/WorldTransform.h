#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	//スケール
	Vector3 scale_ = {1.0f, 1.0f, 1.0f};
	//回転
	Vector3 rotate_ = {0.0f, 0.0f, 0.0f};
	//座標
	Vector3 translate_ = {0.0f, 0.0f, 0.0f};
	// ローカル → ワールド変換行列
	Matrix4x4 matWorld_;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;


	

	/// 初期化
	void Initialize();
	
	/// 行列を計算・転送する
	void Update();
	 
};


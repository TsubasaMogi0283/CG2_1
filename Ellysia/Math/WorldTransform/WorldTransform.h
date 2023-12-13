#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

#include "DirectXSetup.h"


//KAMATA ENGINEを参考にしたがこれを作る意味が分からない
struct ConstBuffDataWorldTransform {
	Matrix4x4 world;
};

struct WorldTransform {
#pragma region メンバ関数

	//初期化
	void Initialize();
	
	//行列を計算・転送する
	void Update();
	
	//転送
	void Transfer();

#pragma endregion

#pragma region メンバ変数

	//スケール
	Vector3 scale_ = {1.0f, 1.0f, 1.0f};
	//回転
	Vector3 rotate_ = {0.0f, 0.0f, 0.0f};
	//座標
	Vector3 translate_ = {0.0f, 0.0f, 0.0f};

	//定数バッファ
	ComPtr<ID3D12Resource> constBuffer_;

	//ワールド行列へ
	Matrix4x4 matWorld_ = {};

	//親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

	#pragma endregion
	
};


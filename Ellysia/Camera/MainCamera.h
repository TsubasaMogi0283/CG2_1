#pragma once
#include <Vector3.h>

#include "DirectXSetup.h"
#include <Matrix4x4.h>

struct MainCamera{
	public:
#pragma region メンバ関数

	//初期化
	void Initialize();
	
	//行列を計算・転送する
	void Update();

private:
	//転送。Updateの中身に入れるよ
	void Transfer();


#pragma endregion

#pragma region メンバ変数


public:
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


	#pragma endregion
	
};

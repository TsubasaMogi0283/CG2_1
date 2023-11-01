#pragma once
#include <Math/Vector/Vector3.h>
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>
#include <Math/Vector/Calculation/VectorCalculation.h>
#include "ViewProjection/ViewProjection.h"
#include "WorldTransform/WorldTransform.h"
#include "Input/Input.h"

class DebugCamera {
private:
	//コンストラクタ
	DebugCamera();

	//デストラクタ
	~DebugCamera();



public:
	
	//インスタンスを取得
	static DebugCamera* GetInstance();

	//初期化
	void Initialize();

	//更新
	void Update();



	ViewProjection GetViewProjection() {
		return viewProjection_;
	}




private:
	Input* input_ = nullptr;


	//必要性はあまりないけど一応Scaleも
	Vector3 scale_ = {};

	//X,Y,Z軸回りのローカル回転角
	Vector3 rotattion_ = { };
	//ローカル座標
	Vector3 translate_ = {};

	//移動の速さ
	const float MOVE_SPEED_AMOUNT_ = 0.1f;
	Vector3 move_ = {};

	//速さ
	const float ROTATE_SPEED_AMOUNT = 0.02f;

	Vector3 offset_ = {};

	//ビュー行列
	ViewProjection viewProjection_ = {};

	//プロジェクションと書いてあるけど
	//Cameraクラスで自動計算するようにしているので
	// そこから取ってくる
	//ワールド座標
	WorldTransform worldTransform_ = {};

	//累積回転行列
	Matrix4x4 matRot_ = MakeIdentity4x4();


};
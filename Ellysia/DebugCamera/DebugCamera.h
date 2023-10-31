#pragma once
#include <Math/Vector/Vector3.h>
#include "ViewProjection/ViewProjection.h"
#include "WorldTransform/WorldTransform.h"

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



	




private:
	//必要性はあまりないけど一応Scaleも
	Vector3 scale_ = {};

	//X,Y,Z軸回りのローカル回転角
	Vector3 rotattion_ = { };
	//ローカル座標
	Vector3 translate_ = {};

	//ビュー行列
	ViewProjection viewProjection_ = {};

	//ワールド座標
	WorldTransform worldTransform_ = {};

	//累積回転行列
	Matrix4x4 matRot_ = {};


};
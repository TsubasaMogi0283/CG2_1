#include "DebugCamera.h"
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

//コンストラクタ
DebugCamera::DebugCamera() {

}

//デストラクタ
DebugCamera::~DebugCamera() {

}


//インスタンスを取得
DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;
	return &instance;
}


//初期化
void DebugCamera::Initialize() {

	scale_ = {1.0f,1.0f,1.0f};
	//X,Y,Z軸回りのローカル回転角
	rotattion_ = { 0.0f,0.0f,0.0f };
	//ローカル座標
	translate_ = { 0.0f,0.0f, - 50.0f };

	matRot_ = MakeIdentity4x4();

	worldTransform_ = {scale_,rotattion_,translate_ };
}

//更新
void DebugCamera::Update() {

	////座標から平行移動行列を計算する
	//Matrix4x4 translateMatrix = MakeTranslateMatrix(worldTransform_.translation_);
	////ワールド行列を計算
	//worldTransform_.matWorld_ = Multiply(matRot_, translateMatrix);



	#pragma endregion

}


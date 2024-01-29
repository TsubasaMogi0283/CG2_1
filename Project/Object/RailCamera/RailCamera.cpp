#include "RailCamera.h"
#include <VectorCalculation.h>
#include "Matrix4x4Calculation.h"
#include <imgui.h>
//コンストラクタ
RailCamera::RailCamera() {

}

//初期化
void RailCamera::Initialize(Vector3 worldPosition, Vector3 radius) {

	worldTransform_.Initialize();
	worldTransform_.rotate_ = radius;
	worldTransform_.translate_ = worldPosition;


	camera_.Initialize();
	camera_.farClip_ = 1200.0f;
}



//更新
void RailCamera::Update() {
	//移動
	Vector3 velocity = { 0.0f,0.0f,0.05f };
	worldTransform_.translate_ = Add(worldTransform_.translate_, velocity);

	//回転
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	worldTransform_.rotate_ = Add(worldTransform_.rotate_, rotate);




	//カメラへ

	worldTransform_.Update();
	//camera_.Update();
	camera_.viewMatrix_ = Inverse(worldTransform_.worldMatrix_);



	ImGui::Begin("RailCamera");
	ImGui::SliderFloat3("translation", &worldTransform_.translate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotate_.x, -10.0f, 10.0f);
	ImGui::End();
}


//デストラクタ
RailCamera::~RailCamera() {

}
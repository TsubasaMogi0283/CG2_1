#include "RailCamera.h"
#include <VectorCalculation.h>
#include "Matrix4x4Calculation.h"
#include <imgui.h>
//コンストラクタ
RailCamera::RailCamera(){

}

//初期化
void RailCamera::Initialize(Vector3 worldPosition,Vector3 radius){
	
	worldTransform_.Initialize();
	worldTransform_.rotate_ = radius;
	worldTransform_.translate_ = worldPosition;


	for (size_t i = 0; i < SEGMENT_COUNT; i++) {
		line_[i] = new Line();
		line_[i]->Initialize();

	}

	

	controlPoints_ = {
		{0.0f,0.0f,0.0f},
		{10.0f,15.0f,0.0f},
		{10.0f,15.0f,0.0f},
		{20.0f,15.0f,0.0f},
		{20.0f,0.0f,0.0f},
		{30.0f,0.0f,0.0f},
	};


	camera_.Initialize();
	camera_.farClip_ = 1200.0f;
}



//更新
void RailCamera::Update(){
	//移動
	Vector3 velocity = { 0.0f,0.0f,0.01f };
	worldTransform_.translate_ = Add(worldTransform_.translate_, velocity);

	//回転
	Vector3 rotate = {0.0f, 0.0f, 0.0f};
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

void RailCamera::Draw(Camera camera) {
	for (size_t i = 0; i < SEGMENT_COUNT + 1; i++) {
		float t = 1.0f / SEGMENT_COUNT * i;
		Vector3 pos = CatmullRom3D(controlPoints_, t);
		//描画用頂点リストに追加
		pointsDrawing.push_back(pos);
	}

	for (int i = 0; i < SEGMENT_COUNT - 1; i++) {
		line_[i]->Draw(pointsDrawing[i], pointsDrawing[i + 1], camera);

	}
}


//デストラクタ
RailCamera::~RailCamera(){
	for (int i = 0; i < SEGMENT_COUNT; i++) {
		delete line_[i];
	}
}
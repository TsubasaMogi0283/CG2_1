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
		{10.0f,10.0f,5.0f},
		{15.0f,15.0f,10.0f},
		{20.0f,10.0f,20.0f},
		{25.0f,5.0f,30.0f},
		{30.0f,0.0f,20.0f},
		{25.0f,10.0f,15.0f},
		{30.0f,15.0f,10.0f},
		{40.0f,10.0f,10.0f},
		{30.0f,5.0f,8.0f},
		{20.0f,0.0f,5.0f},
		{10.0f,5.0f,0.0f},
		{0.0f,0.0f,0.0f},


	};

	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < SEGMENT_COUNT + 1; i++) {
		t = 1.0f / SEGMENT_COUNT * i;
		Vector3 pos = CatmullRom3D(controlPoints_, t);
		// 描画用頂点リストに追加
		pointsDrawing.push_back(pos);
	}

	camera_.Initialize();
}



//更新
void RailCamera::Update(){

	//eyeをワールドトランスフォームの座標に入れる
	worldTransform_.translate_ = eyePosition_;


	if (eyePoint < SEGMENT_COUNT) {

		if (t > 1.0f) {
			t = 0.0f;
			eyePoint++;
			if (targetPoint < SEGMENT_COUNT) {
				targetPoint++;
			}
			if (forwardPoint < SEGMENT_COUNT) {
				forwardPoint++;
			}
		}
		if (t <= 1.0f) {
			eyePosition_ = Leap(pointsDrawing[eyePoint], pointsDrawing[targetPoint], t);
			worldTransform_.translate_ = eyePosition_;
			targetPosition_ = Leap(pointsDrawing[targetPoint], pointsDrawing[forwardPoint], t);
			t += 0.03f;
		}
	}

	//targetとeyeの差分ベクトル(forward)から02_09_ex1より
	//回転角を求めてワールドトランスフォームの角度に入れる
	Vector3 forward = Subtract(targetPosition_, eyePosition_);

	//atan(高さ,底辺)
	//ここは09aとだいたい同じ
	worldTransform_.rotate_.y=std::atan2(forward.x, forward.z);
	//三角比
	float velocityXZ = sqrt((forward.x * forward.x) + (forward.z * forward.z));
	worldTransform_.rotate_.x = std::atan2(-forward.y, velocityXZ);

	//カメラへ
	worldTransform_.worldMatrix_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotate_, worldTransform_.translate_);
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
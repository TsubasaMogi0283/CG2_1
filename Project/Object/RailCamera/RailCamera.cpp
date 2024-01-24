#include "RailCamera.h"
#include <VectorCalculation.h>
#include "Matrix4x4Calculation.h"
#include <imgui.h>
//コンストラクタ
RailCamera::RailCamera(){

}

//初期化
void RailCamera::Initialize(Camera& camera,Vector3 worldPosition,Vector3 radius){
	
	worldTransform_.Initialize();
	worldTransform_.rotate_ = radius;
	worldTransform_.translate_ = worldPosition;


	for (size_t i = 0; i < SEGMENT_COUNT; i++) {
		line_[i] = new Line();
		line_[i]->Initialize();

	}

	

	controlPoints_ = {
		{0,  0,  0 },
		{0,  10, 20},
		{10, 10, 10},
	};


	for (int i = 0; i < POINT_AMOUNT_; i++) {
		pointModel_[i] = std::make_unique<Model>();
		pointModel_[i].reset(Model::Create("Resources/Sample/Player", "playre.obj"));

		pointWorldTransform_[i].Initialize();
		const float POINT_SCALE = 1.0f;
		pointWorldTransform_[i].scale_ = { POINT_SCALE,POINT_SCALE,POINT_SCALE };
		pointWorldTransform_[i].rotate_ = { 0.0f,0.0f,0.0f };
		

	}
	pointWorldTransform_[0].translate_ = controlPoints_[0];
	pointWorldTransform_[1].translate_ = controlPoints_[1];
	pointWorldTransform_[2].translate_ = controlPoints_[2];


	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < SEGMENT_COUNT + 1; i++) {
		t = 1.0f / SEGMENT_COUNT * i;
		Vector3 pos = CatmullRom3D(controlPoints_, t);
		// 描画用頂点リストに追加
		pointsDrawing.push_back(pos);
	}

	camera_.Initialize();
	camera_ = camera;
}

Vector3 RailCamera::CatmullRomPosition(const std::vector<Vector3>& points, float t){
	assert(points.size() >= 4 && "静ぢょてんは4点以上必要です");

	//区間数は制御点の数-1
	size_t division = points.size() - 1;
	//1区間の長さ(全体を1.0とした割合)
	float areaWidth = 1.0f / division;

	//区間内の始点を0.0f、終点を1.0としたときの現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	//下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = Clamp(t_2, 0.0f, 1.0f);

	size_t index = static_cast<size_t>(t / areaWidth);
	//区間番号が上限を超えないための計算
	//index

	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;




	return CatmullRom3D();
}

//更新
void RailCamera::Update(){

	if (eyePoint < SEGMENT_COUNT) {


		//点から点へ移動
		//次の点についたときに次の点に向かう
		//つまりeyePointとtargetPointが1ずつ増えていく


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
		if (t>=0.0f && t <= 1.0f) {
			eyePosition_ = Leap(controlPoints_[eyePoint], controlPoints_[targetPoint], t);
			worldTransform_.translate_ = eyePosition_;
			targetPosition_ = Leap(controlPoints_[targetPoint], controlPoints_[forwardPoint], t);
			t += 0.01f;
		}


		

	}



	//targetとeyeの差分ベクトル(forward)から02_09_ex1より
		//回転角を求めてワールドトランスフォームの角度に入れる
	Vector3 toTarget = Subtract(targetPosition_, eyePosition_);

	//atan(高さ,底辺)
	//ここは09aとだいたい同じ
	worldTransform_.rotate_.y = std::atan2(toTarget.x, toTarget.z);
	//三角比
	float velocityXZ = sqrtf((toTarget.x * toTarget.x) + (toTarget.z * toTarget.z));
	worldTransform_.rotate_.x = std::atan2(-toTarget.y, velocityXZ);

	//カメラへ
	worldTransform_.worldMatrix_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotate_, worldTransform_.translate_);
	//camera_.Update();
	camera_.viewMatrix_ = Inverse(worldTransform_.worldMatrix_);




	
	

	ImGui::Begin("RailCamera");
	ImGui::SliderFloat3("translation", &worldTransform_.translate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotate_.x, -10.0f, 10.0f);
	ImGui::InputInt("targetPoint", &targetPoint);
	ImGui::InputInt("forwardPoint", &forwardPoint);
	ImGui::InputFloat("T", &t);
	
	ImGui::End();
}

void RailCamera::Draw(Camera& camera) {
	//for (size_t i = 0; i < SEGMENT_COUNT + 1; i++) {
	//	float t = 1.0f / SEGMENT_COUNT * i;
	//	Vector3 pos = CatmullRom3D(controlPoints_, t);
	//	//描画用頂点リストに追加
	//	pointsDrawing.push_back(pos);
	//}

	for (int i = 0; i < SEGMENT_COUNT - 1; i++) {
		line_[i]->Draw(pointsDrawing[i], pointsDrawing[i + 1], camera);

	}





	for (int i = 0; i < POINT_AMOUNT_; i++) {
		pointWorldTransform_[i].Update();
	}
	//ポイントにモデル入れてみる
	for (int i = 0; i < POINT_AMOUNT_; i++) {
		pointModel_[i]->Draw(pointWorldTransform_[i], camera);
	}

}




//デストラクタ
RailCamera::~RailCamera(){
	for (int i = 0; i < SEGMENT_COUNT; i++) {
		delete line_[i];
	}
}
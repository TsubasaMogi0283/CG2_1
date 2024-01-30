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
		{0.0f, 0.0f,  0.0f },
		{0.0f, 10.0f, 10.0f},
		{5.0f, 20.0f, 15.0f},
		{10.0f, 25.0f, 20.0f},
		{5.0f, 20.0f, 15.0f},
		//{5.0f, 30.0f, 15.0f},
		//{5.0f, 15.0f, 15.0f},
		//{10.0f, 15.0f, 10.0f},
		//{10.0f, 15.0f, 10.0f},
		//{10.0f, 10.0f, 10.0f},
		//{10.0f, 10.0f, 5.0f},
		//{5.0f, 5.0f, 5.0f},
		//{5.0f, 5.0f, 0.0f},
		//{0.0f, 0.0f, 0.0f},
	};

	//デバッグ用
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



	camera_.Initialize();
	camera_ = camera;
}

Vector3 RailCamera::CatmullRomPosition(const std::vector<Vector3>& points, float t){

	assert(points.size() >= 4 && "制御点は4点以上必要です");

	//区間数は制御点の数-1
	//初期化処理の所のcontrolPointに入っている数を参照してあげる
	size_t division = points.size() - 1;
	//1区間の長さ(全体を1.0とした割合)
	float areaWidth = 1.0f / division;

	//区間内の始点を0.0f、終点を1.0としたときの現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	//下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = Clamp(t_2, 0.0f, 1.0f);

	int index = static_cast<int>(t / areaWidth);
	//区間番号が上限を超えないための計算
	//index = max(index, 4);

	if (index == 3)
	{
		static int a = 0;
		a++;
	}


	int index0 = index - 1;
	int index1 = index;
	int index2 = index + 1;
	int index3 = index + 2;


	
	//始点&終点だった場合制御点を設定し直すよ
	//最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = index1;
	}

	/*if (index > 0 && index < points.size()) {
		index0 = index - 1;
		index1 = index;
		index2 = index + 1;
		index3 = index + 2;
	}*/

	//最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = index2;

		//また最初に戻る
		if (t_ > 1.0f) {
			t_ = 0.0f;
			index = 0;
			index0 = index;
			index1 = index;
			index2 = index + 1;
			index3 = index + 2;
		}
		
	}



	//4点の座標
	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];



	ImGui::Begin("Index");
	ImGui::InputFloat("t2", &t_2);
	ImGui::InputInt("Index", &index);
	ImGui::InputInt("Index0", &index0);
	ImGui::InputInt("Index1", &index1);
	ImGui::InputInt("Index2", &index2);
	ImGui::InputInt("Index3", &index3);
	ImGui::End();


	return CatmullRom3D(p0,p1,p2,p3,t_2);
}

//更新
void RailCamera::Update(){

	//controlPointの上限数に達したとき
	//また最初から




	
	//
	t_ += 0.001f;
	worldTransform_.translate_ = CatmullRomPosition(controlPoints_, t_);
	
	


	worldTransform_.worldMatrix_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotate_, worldTransform_.translate_);
	//camera_.Update();
	camera_.viewMatrix_ = Inverse(worldTransform_.worldMatrix_);




	
	

	ImGui::Begin("RailCamera");
	ImGui::InputFloat("t", &t_);
	ImGui::SliderFloat3("translation", &worldTransform_.translate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotate_.x, -10.0f, 10.0f);
	ImGui::InputInt("targetPoint", &targetPoint);
	ImGui::InputInt("forwardPoint", &forwardPoint);
	ImGui::InputFloat("T", &t);
	
	ImGui::End();
}

void RailCamera::Draw(Camera& camera) {


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
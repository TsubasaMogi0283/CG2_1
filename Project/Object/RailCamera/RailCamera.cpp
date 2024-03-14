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
	worldTransform_.scale_ = {1.0f,1.0f,1.0f};
	worldTransform_.rotate_ = radius;
	worldTransform_.translate_ = worldPosition;

	speed_ = { 0.0f,0.0f,0.0f };
}



//更新
void RailCamera::Update(){
	
	worldTransform_.translate_ = Add(worldTransform_.translate_, speed_);
	worldTransform_.rotate_ = Add(worldTransform_.rotate_, { 0.0f,0.0f,0.0f });

	worldTransform_.Update();
	

	Camera::GetInstance()->SetMatrix(Inverse(worldTransform_.matWorld_));



}


//デストラクタ
RailCamera::~RailCamera(){

}
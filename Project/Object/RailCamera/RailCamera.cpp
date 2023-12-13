#include "RailCamera.h"
#include <VectorCalculation.h>
#include "Matrix4x4Calculation.h"
#include <imgui.h>
//コンストラクタ
RailCamera::RailCamera(){

}

//初期化
void RailCamera::Initialize(Vector3 worldPosition,Vector3 radius){
	
	transform_.scale = {1.0f,1.0f,1.0f};
	transform_.rotate = radius;
	transform_.translate = worldPosition;

}



//更新
void RailCamera::Update(){
	//移動
	Vector3 velocity = { 0.0f,0.0f,-0.02f };
	transform_.translate = Add(transform_.translate, velocity);

	//回転
	Vector3 rotate = {0.0f, 0.0f, 0.0f};
	transform_.rotate = Add(transform_.rotate, rotate);


	

	//カメラへ
	Camera::GetInstance()->SetTranslate(transform_.translate);
	Camera::GetInstance()->SetRotate(transform_.rotate);





	worldMatrix_ = MakeAffineMatrix(
		transform_.scale,
		Camera::GetInstance()->GetRotate(), 
		Camera::GetInstance()->GetTranslate());



	Matrix4x4 viewMatrix = Inverse(worldMatrix_);
	Camera::GetInstance()->Camera::SetViewMatrix(viewMatrix);


	ImGui::Begin("RailCamera");
	ImGui::SliderFloat3("translation", &transform_.translate.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("rotation", &transform_.rotate.x, -10.0f, 10.0f);
	ImGui::End();
}


//デストラクタ
RailCamera::~RailCamera(){

}
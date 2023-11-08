#include "EnemyBullet.h"
#include <Math/Vector/Calculation/VectorCalculation.h>

EnemyBullet::EnemyBullet(){

}

void EnemyBullet::Initialzie(Vector3 position){
	


	model_ = new Model();
	model_->CreateObject("Resources/Sample/cube", "cube.obj");

	const float SCALE_ = 0.3f;
	transform_.scale = {SCALE_,SCALE_,SCALE_};
	transform_.translate = position;

	velocity_ = { 0.0f,0.0f,-0.1f };

}

void EnemyBullet::Update(){
	transform_.translate = Add(transform_.translate, velocity_);
	model_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

	ImGui::Begin("EnemyBullet");
	ImGui::InputFloat3("Translate", &transform_.translate.x);
	ImGui::End();

}

void EnemyBullet::Draw(){
	model_->Draw(transform_);
}

EnemyBullet::~EnemyBullet(){
	delete model_;

}

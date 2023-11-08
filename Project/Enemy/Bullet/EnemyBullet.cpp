#include "EnemyBullet.h"
#include <Math/Vector/Calculation/VectorCalculation.h>

EnemyBullet::EnemyBullet(){

}

void EnemyBullet::Initialzie(){
	


	model_ = new Model();
	model_->CreateObject("Resources/Sample/Enemy", "enemy.obj");

	transform_.scale = {1.0f,1.0f,1.0f};
	transform_.translate = {0.0f,0.0f,9.0f};

	velocity_ = { 0.0f,0.0f,-0.05f };

}

void EnemyBullet::Update(){
	transform_.translate = Add(transform_.translate, velocity_);

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

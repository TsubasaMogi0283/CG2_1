#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){

}

void PlayerBullet::Initialize(Vector3 position){
	//良い感じなの無かったからとりあえずこれで
	//真っ黒
	model_ = new Model();
	model_->CreateObject("Resources/Sample/Enemy", "enemy.obj");
	transform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},position };
}

void PlayerBullet::Update(){
	ImGui::Begin("Bullet");
	ImGui::SliderFloat3("Scale", &transform_.scale.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &transform_.rotate.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &transform_.translate.x, -10.0f, 10.0f);

	ImGui::End();



}

void PlayerBullet::Draw(){
	model_->Draw(transform_);
}

PlayerBullet::~PlayerBullet(){
	delete model_;
}

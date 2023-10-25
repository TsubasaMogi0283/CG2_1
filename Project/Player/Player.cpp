#include "Player.h"

//コンストラクタ
Player::Player() {

}

//初期化
void Player::Initialize() {
	//textureManager_ = TextureManager::GetInstance();

	model_ = new Model();
	model_->CreateObject("Resources/Sample/Enemy", "enemy.obj");

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

}

//更新
void Player::Update() {
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Scale", &transform_.scale.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &transform_.rotate.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Scale", &transform_.translate.x, -1.0f, 1.0f);

	ImGui::End();
}

//描画
void Player::Draw() {
	//model_->Draw(transform_);
}


//デストラクタ
Player::~Player() {
	delete model_;
}


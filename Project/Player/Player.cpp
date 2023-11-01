#include "Player.h"

//コンストラクタ
Player::Player() {

}

//初期化
void Player::Initialize() {

	model_ = new Model();
	model_->CreateObject("Resources/Sample/cube", "cube.obj");

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	input_ = Input::GetInstance();
}

//更新
void Player::Update() {
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Scale", &transform_.scale.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &transform_.rotate.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &transform_.translate.x, -10.0f, 10.0f);

	ImGui::End();

	if (input_->IsPushKey(DIK_UP) == true) {
		transform_.translate.y += MOVE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_DOWN) == true) {
		transform_.translate.y -= MOVE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_RIGHT) == true) {
		transform_.translate.x += MOVE_AMOUNT_;
	}
	if (input_->IsPushKey(DIK_LEFT) == true) {
		transform_.translate.x -= MOVE_AMOUNT_;
	}

}

//描画
void Player::Draw() {
	model_->Draw(transform_);
}


//デストラクタ
Player::~Player() {
	delete model_;
}


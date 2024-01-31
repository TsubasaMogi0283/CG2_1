#include "Player.h"
#include <Object/Collider/CollisionConfig.h>
#include <VectorCalculation.h>
#include "SampleScene/SampleScene.h"
#include "WindowsSetup.h"

//コンストラクタ
Player::Player() {

}

//初期化
void Player::Initialize() {

	model_ = std::unique_ptr<Model>();
	model_.reset(Model::Create("Resources/Sample/Player", "playre.obj"));

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
	//高さのオフセット
	const float HEIGHT_OFFSET = 1.0f;
	worldTransform_.translate_ = {0.0f,HEIGHT_OFFSET,30.0f};

	

	radius_ = 1.0f;


}

void Player::Rotate() {
	if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		worldTransform_.rotate_.z += ROTATE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
		worldTransform_.rotate_.z -= ROTATE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
		worldTransform_.rotate_.y += ROTATE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
		worldTransform_.rotate_.y -= ROTATE_AMOUNT_;
	}
}

void Player::Move() {

	Vector3 move = { 0.0f,0.0f,0.0f };

	//左スティック
	

	if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		move.z += MOVE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		move.z -= MOVE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		move.x += MOVE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		move.x -= MOVE_AMOUNT_;
	}

	const float MOVE_LIMIT_X = 50.0f;
	const float MOVE_LIMIT_Z = 50.0f;

	worldTransform_.translate_ = Add(worldTransform_.translate_, move);


	worldTransform_.translate_.x = max(worldTransform_.translate_.x, -MOVE_LIMIT_X);
	worldTransform_.translate_.x = min(worldTransform_.translate_.x, MOVE_LIMIT_X);
	worldTransform_.translate_.z = max(worldTransform_.translate_.z, -MOVE_LIMIT_Z);
	worldTransform_.translate_.z = min(worldTransform_.translate_.z, MOVE_LIMIT_Z);

}



Vector3 Player::GetWorldPosition() {
	Vector3 result = {};
	//移動成分を取り出してね
	//一番下の行ね
	result.x = worldTransform_.worldMatrix_.m[3][0];
	result.y = worldTransform_.worldMatrix_.m[3][1];
	result.z = worldTransform_.worldMatrix_.m[3][2];

	return result;
}

//更新
void Player::Update(Camera& camera) {
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Scale", &worldTransform_.scale_.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &worldTransform_.rotate_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &worldTransform_.translate_.x, -10.0f, 10.0f);

	ImGui::End();

	Rotate();
	Move();


	worldTransform_.Update();
	
	
}

//描画
void Player::Draw(Camera& camera) {
	
	//本体
	model_->Draw(worldTransform_, camera);


}


//デストラクタ
Player::~Player() {
}


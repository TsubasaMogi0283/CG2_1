#include "Player.h"
#include <Object/Collider/CollisionConfig.h>
#include <VectorCalculation.h>

//コンストラクタ
Player::Player() {

}

//初期化
void Player::Initialize(Vector3 position) {

	model_ = std::unique_ptr<Model>();
	model_.reset(Model::Create("Resources/CG3/Sphere", "Sphere.obj"));

	//textureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");

	worldTransform_.Initialize();
	const float SCALE = 3.0f;
	worldTransform_.scale_ = { SCALE,SCALE,SCALE };
	worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translate_ = position;

	directionalLight_.y = -1.0f;

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
	if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		worldTransform_.translate_.y += MOVE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		worldTransform_.translate_.y -= MOVE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		worldTransform_.translate_.x += MOVE_AMOUNT_;
	}
	if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		worldTransform_.translate_.x -= MOVE_AMOUNT_;
	}

	const float MOVE_LIMIT_X = 17.0f;
	const float MOVE_LIMIT_Y = 7.0f;

	worldTransform_.translate_.x = max(worldTransform_.translate_.x, -MOVE_LIMIT_X);
	worldTransform_.translate_.x = min(worldTransform_.translate_.x, MOVE_LIMIT_X);
	worldTransform_.translate_.y = max(worldTransform_.translate_.y, -MOVE_LIMIT_Y);
	worldTransform_.translate_.y = min(worldTransform_.translate_.y, MOVE_LIMIT_Y);

	//資料にはMatrix沢山あるけど
	//Modelで勝手に計算してくれるようにしているから気にしないでね

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
void Player::Update() {


	
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Scale", &worldTransform_.scale_.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &worldTransform_.rotate_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &worldTransform_.translate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("DirectionalLight", &directionalLight_.x, -1.0f, 1.0f);
	ImGui::End();

	model_->SetScale(worldTransform_.scale_);
	model_->SetRotate(worldTransform_.rotate_);
	model_->SetTranslate(worldTransform_.translate_);
	model_->SetDirection(directionalLight_);

	worldTransform_.Update();


	Rotate();
	Move();

}

//描画
void Player::Draw(Camera& camera) {
	
	model_->Draw(worldTransform_, camera);
	
	
}


//デストラクタ
Player::~Player() {

}


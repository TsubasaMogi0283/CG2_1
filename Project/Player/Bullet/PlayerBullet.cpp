#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(){

}

void PlayerBullet::Initialize(Vector3 position,Vector3 velocity){
	//良い感じなの無かったからとりあえずこれで
	//真っ黒
   	model_ = new Model();
   	model_->CreateObject("Resources/Sample/cube", "cube.obj");
	transform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},position };
	velocity_ = velocity;
}

void PlayerBullet::OnCollision(){
	//デスフラグを立てる
	isDead_ = true;
}

Matrix4x4 PlayerBullet::GetMatrix() {
	Matrix4x4 result = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	return result;
}

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 result = {};
	//移動成分を取り出してね
	//一番下の行ね
	result.x = GetMatrix().m[3][0];
	result.y = GetMatrix().m[3][1];
	result.z = GetMatrix().m[3][2];

	return result;
}

void PlayerBullet::Update(){
	ImGui::Begin("Bullet");
	ImGui::SliderFloat3("Scale", &transform_.scale.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &transform_.rotate.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &transform_.translate.x, -10.0f, 10.0f);

	ImGui::End();

	transform_.translate = Add(transform_.translate, velocity_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(){
	model_->Draw(transform_);
}

PlayerBullet::~PlayerBullet(){
	delete model_;
}

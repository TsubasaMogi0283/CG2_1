#include "PlayerBullet.h"
#include <Collider/CollisionConfig.h>
#include <VectorCalculation.h>

PlayerBullet::PlayerBullet(){

}

void PlayerBullet::Initialize(Vector3 position,Vector3 velocity){
	//良い感じなの無かったからとりあえずこれで
	//真っ黒
   	model_ = std::make_unique<Model>();
   	model_.reset(Model::Create("Resources/Sample/cube", "cube.obj"));
	transform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},position };
	radius_ = 1.0f;
	velocity_ = velocity;

	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY);
}

void PlayerBullet::OnCollision(){
	//デスフラグを立てる
	isDead_ = true;
}

Matrix4x4 PlayerBullet::GetMatrix() {
	Matrix4x4 result = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);
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
	ImGui::SliderFloat3("Scale", &transform_.scale_.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &transform_.rotate_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &transform_.translate_.x, -10.0f, 10.0f);

	ImGui::End();

	transform_.translate_ = Add(transform_.translate_, velocity_);


	transform_.Update();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(){
	model_->Draw();
}

PlayerBullet::~PlayerBullet(){
}

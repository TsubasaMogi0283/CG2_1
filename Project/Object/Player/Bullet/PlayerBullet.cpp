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
	//worldTransform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},position };
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translate_ = position;


	radius_ = 1.0f;
	velocity_ = velocity;

	SetCollisionAttribute(COLLISION_ATTRIBUTE_PLAYER);
	SetCollisionMask(COLLISION_ATTRIBUTE_ENEMY);
}

void PlayerBullet::OnCollision(){
	//デスフラグを立てる
	isDead_ = true;
}


Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 result = {};
	//移動成分を取り出してね
	//一番下の行ね
	result.x = worldTransform_.matWorld_.m[3][0];
	result.y = worldTransform_.matWorld_.m[3][1];
	result.z = worldTransform_.matWorld_.m[3][2];

	return result;
}

void PlayerBullet::Update(){

	model_->SetColor(color_);
	ImGui::Begin("Bullet");
	ImGui::SliderFloat3("Scale", &worldTransform_.scale_.x, 1.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &worldTransform_.rotate_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("Translate", &worldTransform_.translate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat4("Color", &color_.x, 0.0f, 1.0f);
	ImGui::End();

	worldTransform_.translate_ = Add(worldTransform_.translate_, velocity_);
	

	worldTransform_.Update();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(){
	model_->Draw(worldTransform_);
}

PlayerBullet::~PlayerBullet(){
}

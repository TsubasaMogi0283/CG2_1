#include "EnemyParticle.h"
#include <TextureManager.h>

EnemyParticle::EnemyParticle()
{
}

void EnemyParticle::Initialize(Vector3 position){
	model_ = std::make_unique<Particle3D>();
	model_->Create("Resources/05_02", "plane.obj");
	int count = 10;
	model_->SetCount(count);

	textureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");

	worldTransform_.Initialize();
	worldTransform_.translate_ = position;
	const float scale = 0.3f;
	worldTransform_.scale_ = {scale,scale,scale};
}

void EnemyParticle::Update(){


	
	
	model_->SetTransparency(transparency_);
	transparency_ -= 0.05f;
	if (transparency_ < 0.0f) {
		isDead_ = true;
	}
	model_->Update();
	worldTransform_.Update();
}

void EnemyParticle::Draw(){
	model_->Draw(textureHandle_);

}

EnemyParticle::~EnemyParticle()
{
}

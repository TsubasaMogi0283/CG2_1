#include "Enemy.h"
#include "Enemy/State/EnemyApproach/EnemyApproach.h"

Enemy::Enemy(){
	
}

void Enemy::Initialize(){
	model_ = new Model();
	model_->CreateObject("Resources/Sample/Enemy", "enemy.obj");

	transform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,1.0f,30.0f} };
	
	state_ = new EnemyApproach();

	//弾
	bullet_ = new EnemyBullet();
	bullet_->Initialzie(transform_.translate);


}



void Enemy::ChangeState(IEnemy* newState) {
	delete state_;
	this->state_ = newState;


}



void Enemy:: Fire() {
	bullet_->Update();
}

void Enemy::Update(){
	//攻撃
	Fire();

	state_->Update(this);

}

void Enemy::Draw(){
	model_->Draw(transform_);
	bullet_->Draw();
}

Enemy::~Enemy(){
	delete model_;
	delete bullet_;
}

#include "Enemy.h"
#include "Enemy/State/EnemyApproach/EnemyApproach.h"

Enemy::Enemy(){
	
}

void Enemy::Initialize(){
	model_ = new Model();
	model_->CreateObject("Resources/Sample/Enemy", "enemy.obj");

	transform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,1.0f,30.0f} };
	
	state_ = new EnemyApproach();
}



void Enemy::ChangeState(IEnemy* newState) {
	delete state_;
	this->state_ = newState;


}




void Enemy::Update(){
	//メンバ関数ポインタに入っている関数を呼び出す
	//(this->*spFuncTable[static_cast<size_t>(phase_)])();

	state_->Update(this);

}

void Enemy::Draw(){
	model_->Draw(transform_);

}

Enemy::~Enemy(){
	delete model_;
}

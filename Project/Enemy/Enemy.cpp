#include "Enemy.h"

Enemy::Enemy(){
	
}

void Enemy::Initialize(){
	model_ = new Model();
	model_->CreateObject("Resources/Sample/Enemy", "enemy.obj");

	transform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,1.0f,30.0f} };
	approachVelocity_ = { 0.0f,0.0f,-0.3f };
	leaveVelocity_ = { 0.05f,0.01f,-0.3f };

	phase_ = Phase::Approach;

}


void Enemy::Approach() {
	transform_.translate = Add(transform_.translate, approachVelocity_);
		
		if (transform_.translate.z < 0.0f) {
			phase_ = Phase::Leave;
		}
}

void Enemy::Leave() {
	transform_.translate = Add(transform_.translate, leaveVelocity_);
}


void Enemy::Update(){
	

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();

		break;
		case Phase::Leave:
		Leave();
		


		break;
	}

}

void Enemy::Draw(){
	model_->Draw(transform_);

}

Enemy::~Enemy(){
	delete model_;
}

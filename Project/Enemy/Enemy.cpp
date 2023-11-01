#include "Enemy.h"

Enemy::Enemy(){
	
}

void Enemy::Initialize(){
	model_ = new Model();
	model_->CreateObject("Resources/Sample/Enemy", "enemy.obj");

	transform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,1.0f,20.0f} };
	velocity_ = { 0.0f,0.0f,-0.3f };

}

void Enemy::Update(){
	transform_.translate = Add(transform_.translate, velocity_);


}

void Enemy::Draw(){
	model_->Draw(transform_);

}

Enemy::~Enemy(){
	delete model_;
}

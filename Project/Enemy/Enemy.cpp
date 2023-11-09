#include "Enemy.h"
#include "Enemy/State/EnemyApproach/EnemyApproach.h"

Enemy::Enemy(){
	
}

void Enemy::Initialize(){
	model_ = new Model();
	model_->CreateObject("Resources/Sample/Enemy", "enemy.obj");

	transform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,1.0f,30.0f} };
	
	state_ = new EnemyApproach();

	num = state_->GetState();

}

void Enemy::Fire() {
	//弾
	EnemyBullet* bullet_ = new EnemyBullet();
	bullet_->Initialzie(transform_.translate);

	//リストへ
	bullets_.push_back(bullet_);
	shotTime_ = FIRE_INTERVAL_;

}


void Enemy::ChangeState(IEnemy* newState) {
	delete state_;
	this->state_ = newState;


}



void Enemy::Update(){
	ImGui::Begin("aaaa");
	ImGui::InputInt("satet", &num);
	ImGui::End();


	if (state_->GetState() == 0) {
		//離脱になるまで発射
		shotTime_ -= 1;
		if (shotTime_ == 0) {
			Fire();

			
		}
		
		

		//デスフラグの立った玉を削除
		bullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});
	}
	//更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
		
	}
	

	state_->Update(this);

}

void Enemy::Draw(){
	model_->Draw(transform_);
	
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw();
	}

	//弾も
	state_->Draw(this);
}

Enemy::~Enemy(){
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

	delete model_;
	delete state_;
	
}

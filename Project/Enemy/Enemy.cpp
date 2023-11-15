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

	FireAndReset();
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

void Enemy::FireAndReset() {
	Fire();
	//発射タイマーをセットする
	timedCalls_.push_back(
		new TimeCall(std::bind(&Enemy::FireAndReset, this), FIRE_INTERVAL_));

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
		//範囲forでリストの全要素について回す
		for (TimeCall* timedCall : timedCalls_) {
			timedCall->Update();
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
	
	

	//終了したタイマーを削除
	//リストを削除するなら「remove_if」だよ！
	timedCalls_.remove_if([](TimeCall* timedCall) {
        if (timedCall->IsFinished()) {
            delete timedCall;
            return true;
        }
        return false;
    });

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
	

	//timedCall_の解放
	for (TimeCall* timedCall : timedCalls_) {
		delete timedCall;
	}

}

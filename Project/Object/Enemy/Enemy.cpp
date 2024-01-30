#include "Enemy.h"
#include "Object/Enemy/State/EnemyApproach/EnemyApproach.h"
#include "Object/Player/Player.h"
#include "Matrix4x4.h"
#include "VectorCalculation.h"
#include <Object/Collider/CollisionConfig.h>

#include "SampleScene/SampleScene.h"

Enemy::Enemy(){
	
}

void Enemy::Initialize(Vector3 position){
	model_ = std::make_unique<Model>();
	model_.reset(Model::Create("Resources/Sample/Enemy", "enemy.obj"));

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translate_ = position;


	state_ = new EnemyApproach();
	radius_ = 1.0f;
	num = state_->GetState();

	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);

	FireAndReset();
}

void Enemy::Fire() {

	assert(player_);
	const float BULLET_SPEED = 0.1f;
	//敵キャラのワールド座標を取得
	
	Vector3 playerPosition = player_->GetWorldPosition();
	Vector3 enemyPosition = GetWorldPosition();
	//敵と自キャラの差分ベクトル
	Vector3 diffenrence = Subtract(playerPosition,enemyPosition);
	//正規化
	Vector3 velocity = Normalize(diffenrence);
	//速さに合わせる
	Vector3 afterVelocity = {
	    velocity.x, 
		velocity.y, 
		velocity.z * BULLET_SPEED
	};
	Matrix4x4 worldTranslate = {};
	worldTranslate = MakeTranslateMatrix(GetTranslate());

	//速度ベクトルを自機の向きに合わせて回転させる
	afterVelocity = TransformNormal(afterVelocity,worldTransform_.worldMatrix_);




	//弾
	/*EnemyBullet* bullet = new EnemyBullet();
	bullet->Initialzie(worldTransform_.translate_,velocity);
	bullet->SetPlayer(player_);
	gameScene_->AddEnemyBullet(bullet);*/
	//リストへ
	//bullets_.push_back(bullet_);
	shotTime_ = FIRE_INTERVAL_;

}


void Enemy::ChangeState(IEnemy* newState) {
	delete state_;
	this->state_ = newState;
	

}

void Enemy::OnCollision(){

}

void Enemy::FireAndReset() {
	Fire();
	//発射タイマーをセットする
	timedCalls_.push_back(
		new TimeCall(std::bind(&Enemy::FireAndReset, this), FIRE_INTERVAL_));

}


Vector3 Enemy::GetWorldPosition() {
	Vector3 result = {};
	//移動成分を取り出してね
	//一番下の行ね
	result.x = worldTransform_.worldMatrix_.m[3][0];
	result.y = worldTransform_.worldMatrix_.m[3][1];
	result.z = worldTransform_.worldMatrix_.m[3][2];

	return result;
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
		/*bullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});*/
	}



	//更新
	/*for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}*/
	
	worldTransform_.Update();
	

	//終了したタイマーを削除
	//リストの中身を削除するなら「remove_if」だよ！
	timedCalls_.remove_if([](TimeCall* timedCall) {
        if (timedCall->IsFinished()) {
            delete timedCall;
            return true;
        }
        return false;
    });

	state_->Update(this);

}

void Enemy::Draw(Camera&camera){
	model_->Draw(worldTransform_, camera);
	
	/*for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}*/

	//弾も
	state_->Draw(this);
}

Enemy::~Enemy(){
	/*for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}*/

	delete state_;
	

	//timedCall_の解放
	for (TimeCall* timedCall : timedCalls_) {
		delete timedCall;
	}

}

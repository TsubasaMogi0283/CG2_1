#include "Enemy.h"
#include "Enemy/State/EnemyApproach/EnemyApproach.h"
#include "Player/Player.h"
#include "Matrix4x4.h"
#include "VectorCalculation.h"
#include <Collider/CollisionConfig.h>
#include "ImGuiManager.h"
#include <Particle3D.h>


Enemy::Enemy(){
	
}

void Enemy::Initialize(const Vector3& position){
	model_ = std::make_unique<Model>();
	model_.reset(Model::Create("Resources/Sample/Enemy", "enemy.obj"));

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
	worldTransform_.translate_ = position;


	//state_ = new EnemyApproach();
	radius_ = 0.5f;
	//num = state_->GetState();

	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);

	//FireAndReset();
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
	afterVelocity = TransformNormal(afterVelocity,worldTransform_.matWorld_ );

	//弾
	//EnemyBullet* bullet_ = new EnemyBullet();
	//bullet_->Initialzie(transform_.translate_,velocity);
	//bullet_->SetPlayer(player_);
	////リストへ
	//bullets_.push_back(bullet_);
	shotTime_ = FIRE_INTERVAL_;

}


void Enemy::ChangeState(IEnemy* newState) {
	

}

void Enemy::OnCollision(){
	isDead_ = true;

	



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
	result.x = worldTransform_.matWorld_.m[3][0];
	result.y = worldTransform_.matWorld_.m[3][1];
	result.z = worldTransform_.matWorld_.m[3][2];

	return result;
}

void Enemy::Update(){
	

	worldTransform_.translate_.z -= 0.3f* speedOffset_;
	//離脱になるまで発射
	shotTime_ -= 1;

	

	if (shotTime_ == 0) {
		Fire();


	}
	//範囲forでリストの全要素について回す
	/*for (TimeCall* timedCall : timedCalls_) {
		timedCall->Update();
	}*/


	//デスフラグの立った玉を削除
	/*bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});*/

	//更新
	/*for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
		
	}*/
	worldTransform_.Update();
	

	//終了したタイマーを削除
	//リストを削除するなら「remove_if」だよ！
	/*timedCalls_.remove_if([](TimeCall* timedCall) {
        if (timedCall->IsFinished()) {
            delete timedCall;
            return true;
        }
        return false;
    });*/

	//state_->Update(this);

}

void Enemy::Draw(){
	if (isDead_ == false) {
		model_->Draw(worldTransform_);

	}
	

	
	//for (EnemyBullet* bullet : bullets_) {
	//	bullet->Draw();
	//}

}

Enemy::~Enemy(){
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

	//timedCall_の解放
	for (TimeCall* timedCall : timedCalls_) {
		delete timedCall;
	}

}

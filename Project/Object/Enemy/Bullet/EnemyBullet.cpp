#include "EnemyBullet.h"
#include <VectorCalculation.h>
#include <Player/Player.h>

#include <cmath>
#include <Collider/CollisionConfig.h>


EnemyBullet::EnemyBullet(){

}

void EnemyBullet::Initialzie(Vector3 position, Vector3 velocity){
	


	model_ = std::make_unique<Model>();
	model_.reset(Model::Create("Resources/Sample/cube", "cube.obj"));

	const float SCALE_ = 0.3f;
	transform_.scale_ = {SCALE_,SCALE_,SCALE_*SCALE_};
	transform_.translate_ = position;
	radius_=1.0f;
	velocity_ = velocity;

	SetCollisionAttribute(COLLISION_ATTRIBUTE_ENEMY);
	SetCollisionMask(COLLISION_ATTRIBUTE_PLAYER);

#pragma region 確認済み
	//アークタンジェント(atan2)の使い方
	//1...高さ
	//2...底辺


	////Y軸回り角度(θy)
	//transform_.rotate.y = std::atan2(velocity_.x, velocity_.z);
	////X軸回りの角度
	////解法2の方が簡単だからこっちでやる
	////XとZの2Dベクトル
	//float velocityXZ = sqrt(velocity_.x*velocity_.x + velocity_.z*velocity_.z);
	//float heightY = -velocity_.y;
	////X軸回りの角度(θx)
	//transform_.rotate.x = std::atan2(heightY, velocityXZ);
#pragma endregion


}

void EnemyBullet::OnCollision(){
	//デスフラグ立てる
	isDead_ = true;
}

//行列を作って返す
Matrix4x4 EnemyBullet::GetMatrix() {
	Matrix4x4 result = MakeAffineMatrix(transform_.scale_, transform_.rotate_, transform_.translate_);
	return result;
}

//ワールド座標
Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 result = {};
	//移動成分を取り出してね
	//一番下の行ね
	result.x = GetMatrix().m[3][0];
	result.y = GetMatrix().m[3][1];
	result.z = GetMatrix().m[3][2];

	return result;
}

void EnemyBullet::Update(){
#pragma region 動作確認済
	////敵弾から自キャラへのベクトルを計算
	//Vector3 toPlayer = Subtract(player_->GetTranslate(),GetTranslate());
	//
	//Normalize(toPlayer);
	//Normalize(velocity_);
	////球面線形補間により、今の速度と自kたらへのベクトルを内挿し、新たな速度とする
	////自分のだとかなり小さくしないとキツイと分かった。
	//velocity_ = Slerp(velocity_, toPlayer, 0.03f);
	//
	//
	//
	////Y軸回り角度(θy)
	//transform_.rotate.y = std::atan2(velocity_.x, velocity_.z);
	////X軸回りの角度
	////解法2の方が簡単だからこっちでやる
	////XとZの2Dベクトル
	//float velocityXZ = sqrt(velocity_.x*velocity_.x + velocity_.z*velocity_.z);
	//float heightY = -velocity_.y;
	////X軸回りの角度(θx)
	//transform_.rotate.x = std::atan2(heightY, velocityXZ);

#pragma endregion

	transform_.translate_ = Add(transform_.translate_, velocity_);
	model_->SetColor({ 1.0f,0.0f,0.0f,1.0f });


	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}


	model_->SetTranslate(transform_.translate_);
	model_->SetScale(transform_.scale_);

	ImGui::Begin("EnemyBullet");
	ImGui::InputFloat3("Translate", &transform_.translate_.x);
	ImGui::End();

}

void EnemyBullet::Draw(){
	//model_->Draw();
}

EnemyBullet::~EnemyBullet(){

}

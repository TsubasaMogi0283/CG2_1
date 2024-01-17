#pragma once
#include "Model.h"
#include "Transform.h"
#include "State/IEnemy/IEnemy.h"

#include "Object/Enemy/Bullet/EnemyBullet.h"
#include "Object/Enemy/TimeCall/TimeCall.h"
#include "Object/Collider/Collider.h"
#include "WorldTransform.h"
#include "Camera.h"

#include <memory>
enum class Phase {
	Approach,
	Leave,
};

//前方宣言
class Player;


class Enemy:public Collider{
public:

	//コンストラクタ
	Enemy();

	//初期化
	void Initialize();
	void ChangeState(IEnemy* newState);
	
	//コールバック
	void OnCollision()override;

	//更新
	void Update();

	//描画
	void Draw(Camera& camera);


	//デストラクタ
	~Enemy();

private:
	void FireAndReset();
	
	void Fire();



public:
	//アクセッサ
	void SetTranslate(Vector3 translate) {
		this->worldTransform_.translate_ = translate;
	}

	Vector3 GetTranslate() {
		return worldTransform_.translate_;
	}
	
	void SetVelocity(Vector3 velocity) {
		this->velocity_ = velocity;
	}

	//資料ではGameSecneだけど自分のではSampleSceneでやっているよ
	//あまり変わらないでしょう
	void SetPlayer(Player* player) {
		player_ = player;
	}


	//ワールド座標
	Vector3 GetWorldPosition()override;

	float GetRadius() {
		return radius_;
	}

	//弾リストを取得
	const std::list<EnemyBullet*> GetBullets() const {
		return bullets_;
	}

private:
	std::unique_ptr<Model> model_ = nullptr;
	WorldTransform worldTransform_ = {};

	Vector3 velocity_ = {};
	Vector3 approachVelocity_ = {};
	Vector3 leaveVelocity_ = {};


	float radius_;

	IEnemy* state_ = nullptr;


	
	std::list<EnemyBullet*> bullets_;

	const int FIRE_INTERVAL_ = 60;
	int shotTime_ = FIRE_INTERVAL_;

	int num = 0;


	//時限発動のリスト
	TimeCall* timeCall_ = nullptr;
	std::list<TimeCall*> timedCalls_;

	Camera camera_ = {};


	Player* player_ = nullptr;

};


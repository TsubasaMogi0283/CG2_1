#pragma once
#include <Polygon/Model/Model.h>
#include <Math/Vector/Transform.h>
#include "State/IEnemy/IEnemy.h"

#include "Enemy/Bullet/EnemyBullet.h"
#include "Enemy/TimeCall/TimeCall.h"
#include "Collider/Collider.h"

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
	void Draw();


	//デストラクタ
	~Enemy();

private:
	void FireAndReset();
	
	void Fire();



public:
	//アクセッサ
	void SetTranslate(Vector3 translate) {
		this->transform_.translate = translate;
	}

	Vector3 GetTranslate() {
		return transform_.translate;
	}
	
	void SetVelocity(Vector3 velocity) {
		this->velocity_ = velocity;
	}

	//資料ではGameSecneだけど自分のではSampleSceneでやっているよ
	//あまり変わらないでしょう
	void SetPlayer(Player* player) {
		player_ = player;
	}

	//行列を作って返す
	Matrix4x4 GetMatrix();

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
	Model* model_ = nullptr;
	Transform transform_ = {};

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




	Player* player_ = nullptr;

};


#pragma once
#include <Polygon/Model/Model.h>
#include <Math/Vector/Transform.h>
#include "State/IEnemy/IEnemy.h"

#include "Enemy/Bullet/EnemyBullet.h"

enum class Phase {
	Approach,
	Leave,
};


class Enemy{
public:

	//コンストラクタ
	Enemy();

	//初期化
	void Initialize();
	void ChangeState(IEnemy* newState);
	

	//更新
	void Update();

	//描画
	void Draw();


	//デストラクタ
	~Enemy();

private:
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

private:
	Model* model_ = nullptr;
	Transform transform_ = {};

	Vector3 velocity_ = {};
	Vector3 approachVelocity_ = {};
	Vector3 leaveVelocity_ = {};




	IEnemy* state_ = nullptr;


	
	std::list<EnemyBullet*> bullets_;

	const int FIRE_INTERVAL_ = 60;
	int shotTime_ = FIRE_INTERVAL_;

	int num = 0;

};


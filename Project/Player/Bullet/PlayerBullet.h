#pragma once
#include <Polygon/Model/Model.h>
#include "Collider/Collider.h"

class PlayerBullet:public Collider{
public:
	//コンストラクタ
	PlayerBullet();

	//初期化
	void Initialize(Vector3 position,Vector3 velocity);

	//コールバック関数
	void OnCollision()override;

	//行列を作って返す
	Matrix4x4 GetMatrix();

	//ワールド座標
	Vector3 GetWorldPosition()override;

	//更新
	void Update();

	//描画
	void Draw();

	//デストラクタ
	~PlayerBullet();


public:
	//アクセッサ
	bool IsDead() const{
		return isDead_;
	}
	float GetRadius() {
		return radius_;
	}

private:
	Model* model_ = nullptr;
	Transform transform_ = {};

	Vector3 velocity_ = {};


	static const int32_t LIFE_TIME_ = 60 * 3;

	float radius_ = 0.0f;
	int32_t deathTimer_ = LIFE_TIME_;
	//デスフラグ
	bool isDead_ = false;

};


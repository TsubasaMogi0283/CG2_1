#pragma once
#include "Model.h"
#include "Collider/Collider.h"
#include "WorldTransform.h"
#include <memory>

class PlayerBullet:public Collider{
public:
	//コンストラクタ
	PlayerBullet();

	//初期化
	void Initialize(Vector3 position,Vector3 velocity);

	//コールバック関数
	void OnCollision()override;

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
	std::unique_ptr<Model> model_ = nullptr;
	WorldTransform worldTransform_;

	Vector3 velocity_ = {};


	static const int32_t LIFE_TIME_ = 60 * 3;

	float radius_ = 0.0f;
	int32_t deathTimer_ = LIFE_TIME_;
	//デスフラグ
	bool isDead_ = false;
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

};


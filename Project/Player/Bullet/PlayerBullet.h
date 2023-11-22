#pragma once
#include <Polygon/Model/Model.h>

class PlayerBullet{
public:
	//コンストラクタ
	PlayerBullet();

	//初期化
	void Initialize(Vector3 position,Vector3 velocity);

	//コールバック関数
	void OnCollision();

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


private:
	Model* model_ = nullptr;
	Transform transform_ = {};

	Vector3 velocity_;


	static const int32_t LIFE_TIME_ = 60 * 3;

	int32_t deathTimer_ = LIFE_TIME_;
	//デスフラグ
	bool isDead_ = false;

};


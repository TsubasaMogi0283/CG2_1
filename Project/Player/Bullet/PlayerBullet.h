#pragma once
#include <Polygon/Model/Model.h>

class PlayerBullet{
public:

	PlayerBullet();

	void Initialize(Vector3 position,Vector3 velocity);

	void Update();

	void Draw();

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


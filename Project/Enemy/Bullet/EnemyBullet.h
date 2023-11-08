#pragma once
#include "Math/Vector/Transform.h"
#include <Polygon/Model/Model.h>

class EnemyBullet{
public:
	EnemyBullet();


	void Initialzie(Vector3 position);

	void Update();

	void Draw();

	~EnemyBullet();



	//アクセッサ
	bool IsDead() const{
		return isDead_;
	}

private:
	Model* model_ = nullptr;

	Transform transform_ = {};
	Vector3 velocity_ = {};

	bool isDead_ = false;

	int deathTimer_ = 120;
};


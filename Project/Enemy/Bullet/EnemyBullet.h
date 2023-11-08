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

private:
	Model* model_ = nullptr;

	Transform transform_ = {};
	Vector3 velocity_ = {};

};


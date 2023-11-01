#pragma once
#include <Polygon/Model/Model.h>

class PlayerBullet{
public:

	PlayerBullet();

	void Initialize(Vector3 position);

	void Update();

	void Draw();

	~PlayerBullet();

private:
	Model* model_ = nullptr;
	Transform transform_ = {};

};


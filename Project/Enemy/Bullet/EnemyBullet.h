#pragma once
#include "Math/Vector/Transform.h"
#include <Polygon/Model/Model.h>



class Player;

class EnemyBullet{
public:
	EnemyBullet();


	void Initialzie(Vector3 position, Vector3 velocity);

	void Update();

	void Draw();

	~EnemyBullet();

	Vector3 GetTranslate() {
		return transform_.translate;
	}


	//アクセッサ
	bool IsDead() const{
		return isDead_;
	}

	void SetPlayer(Player* player) {
		player_ = player;
	}

private:
	Model* model_ = nullptr;

	Transform transform_ = {};
	Vector3 velocity_ = {};

	bool isDead_ = false;

	int deathTimer_ = 120;

	Player* player_ = nullptr;
};


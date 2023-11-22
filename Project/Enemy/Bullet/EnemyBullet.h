#pragma once
#include "Math/Vector/Transform.h"
#include <Polygon/Model/Model.h>



class Player;

class EnemyBullet{
public:
	//コンストラクタ
	EnemyBullet();

	//初期化
	void Initialzie(Vector3 position, Vector3 velocity);

	//コールバック関数
	void OnCollision();

	//更新
	void Update();

	//描画
	void Draw();

	//デストラクタ
	~EnemyBullet();

	


#pragma region アクセッサ
	Vector3 GetTranslate() {
		return transform_.translate;
	}

	bool IsDead() const{
		return isDead_;
	}

	void SetPlayer(Player* player) {
		player_ = player;
	}

	//行列を作って返す
	Matrix4x4 GetMatrix();

	//ワールド座標
	Vector3 GetWorldPosition();

	//半径
	float GetRadius() {
		return radius_;
	}

#pragma endregion

private:
	Model* model_ = nullptr;

	Transform transform_ = {};
	Vector3 velocity_ = {};
	float radius_;

	bool isDead_ = false;

	int deathTimer_ = 120;

	Player* player_ = nullptr;
};


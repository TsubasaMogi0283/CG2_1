#pragma once
#include "Transform.h"
#include "Model.h"
#include "Collider/Collider.h"

#include <memory>
#include "WorldTransform.h"

class Player;

class EnemyBullet:public Collider{
public:
	//コンストラクタ
	EnemyBullet();

	//初期化
	void Initialzie(Vector3 position, Vector3 velocity);

	//コールバック関数
	void OnCollision()override;

	//更新
	void Update();

	//描画
	void Draw();

	//デストラクタ
	~EnemyBullet();

	


#pragma region アクセッサ
	Vector3 GetTranslate() {
		return worldTransform_.translate_;
	}

	bool IsDead() const{
		return isDead_;
	}

	void SetPlayer(Player* player) {
		player_ = player;
	}

	//ワールド座標
	Vector3 GetWorldPosition()override;

	//半径
	float GetRadius() {
		return radius_;
	}

#pragma endregion

private:
	std::unique_ptr<Model> model_ = nullptr;

	WorldTransform worldTransform_;
	Vector3 velocity_ = {};
	float radius_;

	bool isDead_ = false;

	int deathTimer_ = 120;

	Player* player_ = nullptr;
};


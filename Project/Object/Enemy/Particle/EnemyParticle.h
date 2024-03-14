#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <memory>
#include <Particle3D.h>

class EnemyParticle{
public:
	//コンストラクタ
	EnemyParticle();

	//初期化
	void Initialize(Vector3 position);


	//更新
	void Update();

	//描画
	void Draw();

	//デストラクタ
	~EnemyParticle();

public:
	//アクセッサ
	bool IsDead() const {
		return isDead_;
	}


private:
	std::unique_ptr<Particle3D> model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0u;

	bool isDead_ = false;
	float transparency_ = 1.0f;
};


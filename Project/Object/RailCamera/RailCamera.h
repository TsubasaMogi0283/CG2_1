#pragma once
#include "Transform.h"
#include "Camera.h"
#include "Matrix4x4.h"
#include <WorldTransform.h>
class RailCamera{
public:
	//コンストラクタ
	RailCamera();

	//初期化
	void Initialize(Vector3 worldPosition,Vector3 radius);

	//更新
	void Update();

	void SetSpeed(Vector3 speed) {
		this->speed_ = speed;
	}

	WorldTransform& GetWorldTransform() {
		return worldTransform_;
	}

	//デストラクタ
	~RailCamera();
private:
	WorldTransform worldTransform_ = {};
	Vector3 speed_ = {};
};


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

	WorldTransform& GetWorldTransform() {
		return worldTransform_;
	};

	Camera GetViewProjection() {
		return camera_;
	};

	//デストラクタ
	~RailCamera();
private:
	WorldTransform worldTransform_ = {};

	Camera camera_ = {};
};


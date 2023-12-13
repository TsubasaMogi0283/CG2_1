#pragma once
#include "Transform.h"
#include "Camera.h"
#include "Matrix4x4.h"
class RailCamera{
public:
	//コンストラクタ
	RailCamera();

	//初期化
	void Initialize(Vector3 worldPosition,Vector3 radius);

	//更新
	void Update();

	Matrix4x4 &GetWorldmatrix() {
		return worldMatrix_;
	}

	//デストラクタ
	~RailCamera();
private:
	Transform transform_ = {};

	Matrix4x4 worldMatrix_ = {};


};


#pragma once
#include "Transform.h"
#include "Camera.h"
#include "Matrix4x4.h"
#include <WorldTransform.h>
#include <Line/Line.h>
#include <Model.h>
class RailCamera{
public:
	//コンストラクタ
	RailCamera();

	//初期化
	void Initialize(Camera& camera, Vector3 worldPosition,Vector3 radius);

	//更新
	void Update();

	void Draw(Camera& camera);


	WorldTransform& GetWorldTransform() {
		return worldTransform_;
	};

	Camera GetViewProjection() {
		return camera_;
	};


	Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

	//デストラクタ
	~RailCamera();
private:
	WorldTransform worldTransform_ = {};

	static const int POINT_AMOUNT_ = 7;
	std::unique_ptr<Model> pointModel_[POINT_AMOUNT_] = { nullptr };
	WorldTransform pointWorldTransform_[POINT_AMOUNT_] = {};



	Camera camera_ = {};
};


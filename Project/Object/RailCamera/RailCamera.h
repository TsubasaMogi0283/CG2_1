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

	static const int POINT_AMOUNT_ = 3;
	std::unique_ptr<Model> pointModel_[POINT_AMOUNT_] = { nullptr };
	WorldTransform pointWorldTransform_[POINT_AMOUNT_] = {};

	static const size_t SEGMENT_COUNT = 4;
	Line* line_[SEGMENT_COUNT] = { nullptr };

	//スプライン曲線
	std::vector<Vector3> controlPoints_ = {};
	std::vector<Vector3> pointsDrawing = {};


	int eyePoint = 0;
	int targetPoint = 1;
	int forwardPoint = 2;
	float t = 0;
	Vector3 eyePosition_ = { 0, 0, 0 };
	Vector3 targetPosition_ = { 0, 0, 0 };


	float t_ = 0.0f;

	Camera camera_ = {};
};


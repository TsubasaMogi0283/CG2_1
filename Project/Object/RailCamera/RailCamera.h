#pragma once
#include "Transform.h"
#include "Camera.h"
#include "Matrix4x4.h"
#include <WorldTransform.h>
#include <Line/Line.h>
class RailCamera{
public:
	//コンストラクタ
	RailCamera();

	//初期化
	void Initialize(Vector3 worldPosition,Vector3 radius);

	//更新
	void Update();

	void Draw(Camera camera);


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


	static const size_t SEGMENT_COUNT = 100;
	Line* line_[SEGMENT_COUNT] = { nullptr };

	//スプライン曲線
	std::vector<Vector3> controlPoints_ = {};
	std::vector<Vector3> pointsDrawing = {};


	Camera camera_ = {};
};


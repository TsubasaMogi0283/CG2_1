#pragma once
#include <Polygon/Model/Model.h>
#include <Math/Vector/Transform.h>
#include "State/IEnemy/IEnemy.h"


enum class Phase {
	Approach,
	Leave,
};


class Enemy{
public:

	//コンストラクタ
	Enemy();

	//初期化
	void Initialize();
	void ChangeState(IEnemy* newState);
	

	//更新
	void Update();

	//描画
	void Draw();


	//デストラクタ
	~Enemy();

private:

	void Approach();
	void Leave();





private:
	Model* model_ = nullptr;
	Transform transform_ = {};

	Vector3 approachVelocity_ = {};
	Vector3 leaveVelocity_ = {};

	Phase phase_;


	IEnemy* state_ = nullptr;


};


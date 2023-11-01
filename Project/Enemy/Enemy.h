#pragma once
#include <Polygon/Model/Model.h>
#include <Math/Vector/Transform.h>
class Enemy{
public:

	//コンストラクタ
	Enemy();

	//初期化
	void Initialize();

	

	//更新
	void Update();

	//描画
	void Draw();


	//デストラクタ
	~Enemy();







private:
	Model* model_ = nullptr;
	Transform transform_ = {};

	Vector3 velocity_ = {};

};


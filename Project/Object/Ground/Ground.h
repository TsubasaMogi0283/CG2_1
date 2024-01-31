#pragma once
#include "Model.h"

#include <memory>
#include <WorldTransform.h>
#include "Camera.h"

class Ground {
public:
	//コンストラクタ
	Ground();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw(Camera& camera);

	//デストラクタ
	~Ground();


private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	//位置
	WorldTransform worldTransform_ = {};

};




#pragma once
#include "Model.h"

#include <memory>
#include <WorldTransform.h>

class Skydome{
public:
	//コンストラクタ
	Skydome();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	//デストラクタ
	~Skydome();


private:
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	//位置
	WorldTransform worldTransform_ = {};

};


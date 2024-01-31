#include "Ground.h"
#include <numbers>


//コンストラクタ
Ground::Ground() {

}

//初期化
void Ground::Initialize() {
	model_ = std::make_unique<Model>();
	model_.reset(Model::Create("Resources/05_02","plane.obj"));

	const float SCALE_SIZE = 100.0f;
	//worldTransform_ = { {SCALE_SIZE,SCALE_SIZE,SCALE_SIZE},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	worldTransform_.Initialize();
	worldTransform_.scale_ = { SCALE_SIZE, SCALE_SIZE, SCALE_SIZE };
	//元々縦だったのでX軸回転させて横にしよう！
	worldTransform_.rotate_ = { float(std::numbers::pi)/2.0f,0.0f,0.0f };
	worldTransform_.translate_ = { 0.0f,0.0f,0.0f };


}

//更新
void Ground::Update() {

	worldTransform_.Update();

}

//描画
void Ground::Draw(Camera& camera) {
	model_->Draw(worldTransform_, camera);
}

//デストラクタ
Ground::~Ground() {

}

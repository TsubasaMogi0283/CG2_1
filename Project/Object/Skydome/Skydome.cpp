#include "Skydome.h"

//コンストラクタ
Skydome::Skydome(){

}

//初期化
void Skydome::Initialize(){
	model_ = std::make_unique<Model>();
	model_.reset(Model::Create("Resources/Sample/CelestialSphere", "CelestialSphere.obj"));
	
	const float SCALE_SIZE = 30.0f;
	//worldTransform_ = { {SCALE_SIZE,SCALE_SIZE,SCALE_SIZE},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	worldTransform_.Initialize();
	worldTransform_.scale_ = { SCALE_SIZE, SCALE_SIZE, SCALE_SIZE };
	worldTransform_.rotate_ = {0.0f,0.0f,0.0f};
	worldTransform_.translate_ = {0.0f,0.0f,0.0f};


}

//更新
void Skydome::Update(){

	worldTransform_.Update();

}

//描画
void Skydome::Draw(){
	model_->Draw(worldTransform_);
}

//デストラクタ
Skydome::~Skydome(){

}

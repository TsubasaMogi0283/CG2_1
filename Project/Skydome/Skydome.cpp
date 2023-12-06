#include "Skydome.h"

//コンストラクタ
Skydome::Skydome(){

}

//初期化
void Skydome::Initialize(){
	model_ = std::make_unique<Model>();
	model_->CreateObject("Resources/Sample/CelestialSphere", "CelestialSphere.obj");
	
	const float SCALE_SIZE = 30.0f;
	transform_ = { {SCALE_SIZE,SCALE_SIZE,SCALE_SIZE},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

//更新
void Skydome::Update(){

}

//描画
void Skydome::Draw(){
	model_->Draw(transform_);
}

//デストラクタ
Skydome::~Skydome(){

}

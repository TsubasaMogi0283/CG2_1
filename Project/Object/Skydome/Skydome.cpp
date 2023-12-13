#include "Skydome.h"

//コンストラクタ
Skydome::Skydome(){

}

//初期化
void Skydome::Initialize(){
	model_ = std::make_unique<Model>();
	model_.reset(Model::Create("Resources/Sample/CelestialSphere", "CelestialSphere.obj"));
	
	const float SCALE_SIZE = 30.0f;
	transform_ = { {SCALE_SIZE,SCALE_SIZE,SCALE_SIZE},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

//更新
void Skydome::Update(){

	model_->SetScale(transform_.scale_);
	model_->SetRotate(transform_.rotate_);
	model_->SetTranslate(transform_.translate_);


}

//描画
void Skydome::Draw(){
	model_->Draw();
}

//デストラクタ
Skydome::~Skydome(){

}

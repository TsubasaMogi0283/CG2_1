#include "Skydome.h"

//コンストラクタ
Skydome::Skydome(){

}

//初期化
void Skydome::Initialize(){
	model_ = std::make_unique<Model>();
	uint32_t modelHandle = ModelManager::GetInstance()->LoadObject("Resources/CelestialSphere", "CelestialSphere.obj");
	model_.reset(Model::Create(modelHandle));
	
	const float SCALE_SIZE = 200.0f;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { SCALE_SIZE, SCALE_SIZE, SCALE_SIZE };
	worldTransform_.rotate_ = {0.0f,0.0f,3.0f};
	worldTransform_.translate_ = {0.0f,0.0f,0.0f};


}

//更新
void Skydome::Update(){

	worldTransform_.Update();

}

//描画
void Skydome::Draw(Camera& camera){
	model_->Draw(worldTransform_, camera);
}

//デストラクタ
Skydome::~Skydome(){

}

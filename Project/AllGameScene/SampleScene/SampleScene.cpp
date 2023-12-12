#include "SampleScene.h"
#include "AllGameScene/GameManager/GameManager.h"
#include "TextureManager/TextureManager.h"
#include <Blend/BlendMode.h>

#include <random>


/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {

	//BlendModeを使う時は各Initializeの前で使ってね
	modelColor_ = {1.0f,1.0f,1.0f,1.0f};
	
	
	
	model_ = std::make_unique<Particle3D>();
	model_->Create("Resources/05_02", "plane.obj");
	emitter_.count = 3;
	
	//0.5秒ごとに発生
	emitter_.frequency = 0.5f;
	//発生頻度用の時刻。0.0で初期化
	emitter_.frequencyTime = 0.0f;



	particleTextureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");

	model_->SetEmitter(emitter_);
	cameraPosition_ = {0.0f,7.0f,-7.0f};
	cameraRotate_ = { 0.8f,0.0f,0.0f };



}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	//カメラ
	Camera::GetInstance()->SetRotate(cameraRotate_);
	Camera::GetInstance()->SetTranslate(cameraPosition_);

	

	model_->SetColor(modelColor_);

	model_->SetScale(scale_);
	model_->SetTranslate(traslate_);


	model_->Update();
	
	
	ImGui::Begin("Model");
	ImGui::SliderFloat3("Scale", &scale_.x, 0.0f, 3.0f);
	ImGui::SliderFloat3("Translate", &traslate_.x, -3.0f, 3.0f);
	ImGui::SliderFloat4("Color", &modelColor_.x, 0.0f, 1.0f);
	
	ImGui::End();

	
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translate", &cameraPosition_.x, -20.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &cameraRotate_.x, -5.0f, 5.0f);
	ImGui::End();

}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	model_->Draw(particleTextureHandle_);


}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	
}

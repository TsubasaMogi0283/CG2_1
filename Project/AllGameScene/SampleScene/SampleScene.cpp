#include "SampleScene.h"
#include "AllGameScene/GameManager/GameManager.h"
#include "TextureManager/TextureManager.h"
#include <Blend/BlendMode.h>

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
	
	
	model_ = Particle3D::Create("Resources/05_02", "plane.obj");
	modelTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	

	cameraPosition_ = {0.0f,3.7f,-8.0f};
	cameraRotate_ = { 0.4f,0.0f,0.0f };



}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	
	//カメラ
	Camera::GetInstance()->SetRotate(cameraRotate_);
	Camera::GetInstance()->SetTranslate(cameraPosition_);


	//particle_->Update();
	//後でSceneなどで変更できるようにしておく
	const float DELTA_TIME = 1.0f / 60.0f;
	traslate_.x += velocity_.x * DELTA_TIME;
	traslate_.y += velocity_.y * DELTA_TIME;
	traslate_.z += velocity_.z * DELTA_TIME;

	model_->SetTranslate(traslate_);


	model_->SetColor(modelColor_);


	ImGui::Begin("Model");
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
	model_->Draw();
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete model_;
}

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
	particle_ = new Particle3D();
	particle_->SetBlendMode(BlendModeNormal);
	particle_->CreateObject("Resources/05_02","plane.obj");
	modelTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	spriteTransform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sprite = new Sprite();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/uvChecker.png");

	sprite->SetBlendMode(BlendModeNormal);
	sprite->LoadTextureHandle(textureHandle);
	sprite->SetAllPosition({ { 0.0f,0.0f }, { 0.0f,512.0f }, { 512.0f,0.0f, }, { 512.0f,512.0f} });

	
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


	particle_->Update();


	sprite->SetColor(spriteColor_);
	particle_->SetColor(modelColor_);

	ImGui::Begin("Sprite");
	ImGui::SliderFloat4("Color", &spriteColor_.x, 0.0f, 1.0f);
	ImGui::End();


	//modelTransform_.rotate.y += 0.05f;
	
	
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translate", &cameraPosition_.x, -20.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &cameraRotate_.x, -5.0f, 5.0f);
	ImGui::End();

}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	particle_->Draw();
	//sprite->DrawRect(spriteTransform_);
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete particle_;
	delete sprite;
}

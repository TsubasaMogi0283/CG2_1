#include "SampleScene.h"
#include "AllGameScene/GameManager/GameManager.h"
#include "TextureManager/TextureManager.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {
	model_ = new Model();
	model_->CreateObject("Resources/05_02","plane.obj");
	modelTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	spriteTransform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sprite = new Sprite();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/monsterBall.png");
	sprite->LoadTextureHandle(textureHandle);
	sprite->SetAllPosition({ { 0.0f,0.0f }, { 0.0f,600.0f }, { 1200.0f,0.0f, }, { 1200.0f,600.0f} });


}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	modelTransform_.rotate.y += 0.05f;

	ImGui::Begin("Plane");
	ImGui::SliderFloat3("Translate", &modelTransform_.translate.x, -10.0f, 10.0f);
	ImGui::End();
	
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	model_->Draw(modelTransform_);
	sprite->DrawRect(spriteTransform_);
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete model_;
	delete sprite;
}

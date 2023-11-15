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
	model_->CreateObject("Resources/05_02", "plane.obj");
	modelTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	sprite = new Sprite();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/uvChecker.png");
	sprite->LoadTextureHandle(textureHandle);
	

}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	sprite->SetFlipX(flipX_);
	sprite->SetFlipY(flipY_);


	sprite->SetAnchorPoint(anchorPoint);
	sprite->SetScale(scale_);
	sprite->SetRotate(rotate);
	sprite->SetPosition(position_);
	modelTransform_.rotate.y += 0.05f;
	sprite->SetColor(color_);
	ImGui::Begin("Plane");
	ImGui::SliderFloat3("Translate", &modelTransform_.translate.x, -10.0f, 10.0f);
	
	ImGui::End();
	

	ImGui::Begin("Sprite");
	ImGui::SliderFloat4("color", &color_.x, 0.0f, 1.0f);
	ImGui::SliderFloat2("Scale", &scale_.x, 0.0f, 3.0f);
	ImGui::SliderFloat("Rotate", &rotate, 0.0f,3.0f);
	ImGui::SliderFloat2("Position", &position_.x, 0.0f,1000.0f);
	ImGui::SliderFloat2("AnchorPoint", &anchorPoint.x, 0.0f,1.0f);
	ImGui::Checkbox("FlipX", &flipX_);
	ImGui::Checkbox("FlipY", &flipY_);


	ImGui::End();


}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	model_->Draw(modelTransform_);
	sprite->DrawRect();
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete model_;
	delete sprite;
}

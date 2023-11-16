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
	model_=Model::Create("Resources/05_02", "plane.obj");
	modelTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	sprite = new Sprite();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/uvChecker.png");
	sprite->LoadTextureHandle(textureHandle);
	

}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

#pragma region Sprite

	sprite->SetFlipX(flipX_);
	sprite->SetFlipY(flipY_);
	

	sprite->SetAnchorPoint(anchorPoint);
	sprite->SetScale(scale_);
	sprite->SetRotate(rotate);
	sprite->SetPosition(position_);
	sprite->SetInvisible(isInvisible_);

	sprite->SetTextureLeftTop(leftTop_);
	sprite->SetTextureSize(size_);
	sprite->SetColor(color_);

	ImGui::Begin("Sprite");
	ImGui::SliderFloat4("color", &color_.x, 0.0f, 1.0f);
	ImGui::SliderFloat2("Scale", &scale_.x, 0.0f, 3.0f);
	ImGui::SliderFloat("Rotate", &rotate, 0.0f,3.0f);
	ImGui::SliderFloat2("Position", &position_.x, 0.0f,1000.0f);
	ImGui::SliderFloat2("AnchorPoint", &anchorPoint.x, 0.0f,1.0f);
	
	ImGui::SliderFloat2("LeftTop", &leftTop_.x, 0.0f,1000.0f);
	ImGui::SliderFloat2("Size", &size_.x, 0.0f,1000.0f);
	

	ImGui::Checkbox("FlipX", &flipX_);
	ImGui::Checkbox("FlipY", &flipY_);
	ImGui::Checkbox("Invisible", &isInvisible_);

	ImGui::End();


#pragma endregion


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
	sprite->DrawRect();
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete sprite;
}

#include "SampleScene.h"
#include "AllGameScene/GameManager/GameManager.h"
#include "TextureManager.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {
	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		model_[i] = Model::Create("Resources/CG3/fence", "fence.obj");
	}
	modelTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	sprite = std::make_unique<Sprite>();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/uvChecker.png");
	
	sprite.reset(Sprite::Create(textureHandle, { 0.0f,0.0f }));

}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {


	modelTransform_.rotate.y += 0.05f;
	model_[0]->SetColor(modelColor_);
	model_[0]->SetTranslate(modelTransform_.translate);

	ImGui::Begin("Plane");
	ImGui::SliderFloat3("Translate", &modelTransform_.translate.x, -10.0f, 10.0f);
	ImGui::SliderFloat4("Color", &modelColor_.x, 0.0f, 1.0f);
	

	ImGui::End();
	

	


}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		model_[i]->Draw();
	
	}
	sprite->Draw();
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		delete model_[i];
	}
}

#include "SampleScene.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {
	player_ = new Player();
	player_->Initialize();



	enemy_ = new Enemy();
	enemy_->Initialize();
	enemy_->SetPlayer(player_);

	uint32_t textureHandle_ = TextureManager::LoadTexture("Resources/uvChecker.png");
	cameraTranslate_ = Camera::GetInstance()->GetTranslate();

	
}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	Camera::GetInstance()->Camera::SetTranslate(cameraTranslate_);


	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &cameraTranslate_.x, -20.0f, 10.0f);

	ImGui::End();


	player_->Update();
	
	enemy_->Update();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	player_->Draw();

	enemy_->Draw();
}


/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete player_;
	delete enemy_;
}

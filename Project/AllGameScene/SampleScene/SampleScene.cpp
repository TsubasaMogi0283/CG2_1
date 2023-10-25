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
	uint32_t textureHandle_ = TextureManager::LoadTexture("Resources/uvChecker.png");
	
}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	player_->Update();

	
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	player_->Draw();
}


/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete player_;
}

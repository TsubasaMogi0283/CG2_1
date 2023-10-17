#include "SampleScene.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {

}

/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {
	

	uint32_t textureHandle_ = TextureManager::LoadTexture("Resources/uvChecker.png");
	uint32_t textureHandle2_= TextureManager::LoadTexture("Resources/monsterBall.png");
	uint32_t textureHandle3_= TextureManager::LoadTexture("Resources/bullet.png");


	transformSprite_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	

	transformSprite2_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{300.0f,0.0f,0.0f} };
	
	
	

	transformSprite3_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{800.0f,0.0f,0.0f} };
	


	

	
}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	//plane_->Draw(transformModel_);
	//sprite_->DrawRect(transformSprite_);
	//sprite2_->DrawRect(transformSprite2_);
	//sprite3_->DrawRect(transformSprite3_);

}

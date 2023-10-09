#include "SampleScene2.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene2::SampleScene2() {

}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene2::~SampleScene2() {
	//sprite_->Release();
	audio_->SoundUnload(&soundData_);
	audio_->DeleteInstance();

	delete sprite_;
}

/// <summary>
/// 初期化
/// </summary>
void SampleScene2::Initialize(GameManager* gameManager) {
	sprite_ = new Sprite();
	sprite_->LoadTexture("Resources/uvChecker.png");

	transformSprite_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{512.0f,0.0f,0.0f} };
	spriteAllPosition_ = { {0.0f,0.0f},{0.0f,512.0f},{512.0f,0.0f},{512.0f,512.0f} };
	sprite_->SetAllPosition(spriteAllPosition_);


	audio_ = Audio::GetInstance();
	audio_->Initialize();
	soundData_ = audio_->LoadWave("Resources/Audio/Sample/Win.wav");


	audio_->PlayWave(soundData_ ,true);
	

	
}

/// <summary>
/// 更新
/// </summary>
void SampleScene2::Update(GameManager* gameManager) {
	ImGui::Begin("SampleScene2");
	ImGui::End();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene2::Draw(GameManager* gameManager) {
	sprite_->DrawRect(transformSprite_);
}

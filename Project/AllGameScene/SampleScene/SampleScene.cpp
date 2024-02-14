#include "SampleScene.h"

#include <list>


/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {
	audio_ = Audio::GetInstance();
	audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/Win.wav");
	audio_->PlayWave(audioHandle_, true);
	//audio_->ChangePitch(audioHandle_, pitch_);
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	

	//audio_->ChangePitch(audioHandle_, pitch_);
	audio_->RatioCalculationDebug();
	audio_->SetPan(audioHandle_, pitch_);

	ImGui::Begin("Audio");
	ImGui::SliderFloat("Pitch", &pitch_,-1.0f,1.0f);
	ImGui::End();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {

}

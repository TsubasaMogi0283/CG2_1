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
	audio_->SetPan(audioHandle_, pan_);
	audio_->ChangePitch(audioHandle_, pitch_);
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	

	audio_->ChangePitch(audioHandle_, pitch_);
	audio_->SetPan(audioHandle_, pan_);


#ifdef _DEBUG
	ImGui::Begin("Audio");
	ImGui::SliderFloat("Pan", &pan_,-1.0f,1.0f);
	ImGui::InputInt("Pitch", &pitch_);
	ImGui::End();
#endif

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

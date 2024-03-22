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
	audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/Game.wav");
	
	audio_->PartlyLoopPlayWave(audioHandle_,2.0f,5.0f);
	audio_->SetPan(audioHandle_, pan_);
	audio_->ChangePitch(audioHandle_, pitch_);
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	//audio_->CreateReverb(audioHandle_);
	audio_->ChangePitch(audioHandle_, pitch_);
	audio_->SetPan(audioHandle_, pan_);

	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		audio_->ExitLoop(audioHandle_);
	}

	if (Input::GetInstance()->IsTriggerKey(DIK_1) == true) {
		audio_->PauseWave(audioHandle_);
	}
	if (Input::GetInstance()->IsTriggerKey(DIK_2) == true) {
		audio_->ResumeWave(audioHandle_);
	}


#ifdef _DEBUG
	ImGui::Begin("Audio");
	ImGui::SliderFloat("Pan", &pan_, -1.0f, 1.0f);
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
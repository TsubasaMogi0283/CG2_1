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
	audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/TestC4.wav");
	audio_->PlayWave(audioHandle_, true);
	audio_->ChangePitch(audioHandle_, pitch_);
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	

	/*if (Input::GetInstance()->IsTriggerKey(DIK_UP) == true) {
		pitch_ += 1;
	}
	else if (Input::GetInstance()->IsTriggerKey(DIK_DOWN) == true) {
		pitch_ -= 1;
	}*/
	audio_->ChangePitch(audioHandle_, pitch_);
	audio_->RatioCalculationDebug();
	//audio_->ChangeSpeed(audioHandle_, pitch_);

	ImGui::Begin("Audio");
	ImGui::InputInt("Pitch", &pitch_);
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

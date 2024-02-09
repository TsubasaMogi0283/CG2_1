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
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	

	if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		pitch_ += 0.005f;
	}
	else if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		pitch_ -= 0.005f;
	}
	audio_->ChangePitch(audioHandle_,pitch_);

	ImGui::Begin("Audio");
	ImGui::InputFloat("Pitch", &pitch_);
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

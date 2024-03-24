#include "ExplanationSceneAudio.h"
#include "GameScene/GameScene.h" 
#include "GameScene/Audio/Play/PlaySceneAudio.h"


ExplanationSceneAudio::ExplanationSceneAudio(){

}

void ExplanationSceneAudio::Initialize(){
	bgm_ = Audio::GetInstance();
	bgmHandle_ = bgm_->LoadWave("Resources/Audio/Ready/ResultBGM.wav");
	bgm_->PlayWave(bgmHandle_, true);
	bgm_->ChangeVolume(bgmHandle_, 0.8f);
}

void ExplanationSceneAudio::Update(GameScene* gameManager){


	//止める
	if (gameManager->GetSceneNo() != 0) {
		bgm_->StopWave(bgmHandle_);
		if (gameManager->GetSceneNo() == 2) {
			gameManager->ChangeScene(new PlaySceneAudio());
		}
	}

}

ExplanationSceneAudio::~ExplanationSceneAudio(){

}

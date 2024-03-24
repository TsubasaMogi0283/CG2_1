#include "PlaySceneAudio.h"

#include "GameScene/GameScene.h"

PlaySceneAudio::PlaySceneAudio(){

}

void PlaySceneAudio::Initialize(){
	bgm_ = Audio::GetInstance();
	bgmHandle_ = bgm_->LoadWave("Resources/Audio/Play/GamePlay.wav");
	bgm_->PlayWave(bgmHandle_, true);
	bgm_->ChangeVolume(bgmHandle_, 0.7f);
}

void PlaySceneAudio::Update(GameScene* gameManager){

	//止める
	if (gameManager->GetSceneNo() != 2) {
		bgm_->StopWave(bgmHandle_);
	}

}

PlaySceneAudio::~PlaySceneAudio()
{
}

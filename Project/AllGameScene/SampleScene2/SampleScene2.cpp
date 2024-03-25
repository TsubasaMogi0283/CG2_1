#include "SampleScene2.h"
#include <TextureManager.h>
#include <Input.h>
#include "SampleScene/SampleScene.h"
#include "GameManager.h"

SampleScene2::SampleScene2(){

}

void SampleScene2::Initialize(){
	sprite = std::make_unique<Sprite>();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/monsterBall.png");
	spritePosition_ = { 100.0f,100.0f };
	sprite.reset(Sprite::Create(textureHandle, spritePosition_));

	audio_ = Audio::GetInstance();
	audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/Game.wav");
	audio_->PlayWave(audioHandle_, true);
}

void SampleScene2::Update(GameManager* gameManager){

	const float VOLUME_AMOUNT = 0.01f;
	if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		volume_ += VOLUME_AMOUNT;
	}
	if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		volume_ -= VOLUME_AMOUNT;
	}

	if (volume_ > 1.0f) {
		volume_ = 1.0f;
	}
	if (volume_ < 0.0f) {
		volume_ = 0.0f;
	}

	audio_->ChangeVolume(audioHandle_, volume_);

	sprite->SetPosition(spritePosition_);

	const float MOVE_AMOUNT = 1.0f;
	const float VELOCITY = 3.0f;
	if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
		spritePosition_.x += MOVE_AMOUNT * VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
		spritePosition_.x -= MOVE_AMOUNT * VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		spritePosition_.y -= MOVE_AMOUNT * VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
		spritePosition_.y += MOVE_AMOUNT * VELOCITY;
	}



	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		audio_->StopWave(audioHandle_);
		gameManager->ChangeScene(new SampleScene());

	}

}

void SampleScene2::Draw(){
	sprite->Draw();
}

SampleScene2::~SampleScene2()
{
}

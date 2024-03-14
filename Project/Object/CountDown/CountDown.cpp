#include "CountDown.h"
#include "TextureManager.h"

CountDown::CountDown(){

}

void CountDown::Initialize(){
	gameTime_ = 60 * timer_;
	displayTime_ = gameTime_ / 60;

	//カウントダウン
	numberTextureHandle[0] = TextureManager::LoadTexture("Resources/Number/0.png");
	numberTextureHandle[1] = TextureManager::LoadTexture("Resources/Number/1.png");
	numberTextureHandle[2] = TextureManager::LoadTexture("Resources/Number/2.png");
	numberTextureHandle[3] = TextureManager::LoadTexture("Resources/Number/3.png");
	numberTextureHandle[4] = TextureManager::LoadTexture("Resources/Number/4.png");
	numberTextureHandle[5] = TextureManager::LoadTexture("Resources/Number/5.png");
	numberTextureHandle[6] = TextureManager::LoadTexture("Resources/Number/6.png");
	numberTextureHandle[7] = TextureManager::LoadTexture("Resources/Number/7.png");
	numberTextureHandle[8] = TextureManager::LoadTexture("Resources/Number/8.png");
	numberTextureHandle[9] = TextureManager::LoadTexture("Resources/Number/9.png");

	//カウントダウン
	uint32_t frameTextureHandle = TextureManager::LoadTexture("Resources/Frame/Frame.png");
	countDownBackPosition_ = { 590.0f,18.0f };
	countDownBackSize_ = { 0.11f,0.12f };
	countDownBack_.reset(Sprite::Create(frameTextureHandle,countDownBackPosition_ ));
	
	countDownBack_->SetScale(countDownBackSize_);
	countDownBack_->SetPosition(countDownBackPosition_);
	for (int i = 0; i < NUMBER_AMOUNT_; i++) {
		//代入の時はresetを使ってね
		timeTensPlane_[i].reset(Sprite::Create(numberTextureHandle[i], { 600.0f,30.0f }));
		timeOnesPlane_[i].reset(Sprite::Create(numberTextureHandle[i], { 680.0f,30.0f }));
	}

}

void CountDown::Update(){
	//仮で60秒
	if (isCountDown_ == true) {
		gameTime_ -= 1;

	}
	
	displayTime_ = gameTime_ / 60;

	tensPlace_ = displayTime_ / 10;
	onesPlace_ = displayTime_ % 10;

	
}

void CountDown::Draw(){
	countDownBack_->Draw();

	//スプライトは後ろに描画してね
	//透明部分がすり抜けてしまうから
	for (int i = 0; i < NUMBER_AMOUNT_; i++) {
		if (tensPlace_ == i) {
			timeTensPlane_[i]->Draw();
		}
		if (onesPlace_ == i) {
			timeOnesPlane_[i]->Draw();
		}
	}
}

CountDown::~CountDown(){

}

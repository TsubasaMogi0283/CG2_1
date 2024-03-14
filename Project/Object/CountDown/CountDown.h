#pragma once
#include "Sprite.h"
#include <memory>

class CountDown{
public:
	//コンストラクタ
	CountDown();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	//デストラクタ
	~CountDown();

public:

	//timeはあくまで初期値だから違うよ
	int GetTime() {
		return gameTime_;
	}

	bool GetIsCountDown() {
		return isCountDown_;
	}

	void IsSetCountDown(bool isCountDown) {
		this->isCountDown_ = isCountDown;
	}

private:

	static const int NUMBER_AMOUNT_ = 10;
	std::unique_ptr<Sprite> timeTensPlane_[NUMBER_AMOUNT_] = { nullptr };
	std::unique_ptr<Sprite> timeOnesPlane_[NUMBER_AMOUNT_] = { nullptr };

	//背景
	std::unique_ptr<Sprite> countDownBack_ = nullptr;
	Vector2 countDownBackPosition_ = {};
	Vector2 countDownBackSize_ = {};

	//ゲームの時間
	const int timer_ = 20;
	int gameTime_ = 60 * timer_;

	//表示されている時間
	int displayTime_ = gameTime_ / 60;
	//1の位
	int onesPlace_ = 0;
	//10の位
	int tensPlace_ = 0;

	uint32_t numberTextureHandle[NUMBER_AMOUNT_] = {};

	//減る
	//負けたらfalseにして減らないようにする
	bool isCountDown_ = true;


};


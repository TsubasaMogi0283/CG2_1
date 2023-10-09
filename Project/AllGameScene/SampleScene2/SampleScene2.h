#pragma once

#include <Input/Input.h>
#include <Audio/Audio.h>
#include "AllGameScene/GameManager/GameManager.h"
#include "AllGameScene/GameManager/IGameScene.h"

//StatePatternをル買う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene2 : public IGameScene {
public:

	//コンストラクタ
	SampleScene2();

	/// デストラクタ
	~SampleScene2();

	/// 初期化
	void Initialize(GameManager* gameManager)override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw(GameManager* gameManager)override;

private:
	//Audio
	Audio* audio_ = nullptr;
	//Input
	Input* input_ = nullptr;
	

	

	Sprite* sprite_ = nullptr;

	SoundData soundData_ = {};

	Transform transformSprite_;
	SpritePosition spriteAllPosition_;

};

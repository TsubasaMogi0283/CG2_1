#pragma once
#include <Input/Input.h>
#include <Audio/Audio.h>
#include "TextureManager/TextureManager.h"
#include "AllGameScene/GameManager/GameManager.h"
#include "AllGameScene/GameManager/IGameScene.h"
#include <Math/Vector/SpritePosition.h>

//StatePatternをル買う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene();

	/// デストラクタ
	~SampleScene();

	/// 初期化
	void Initialize(GameManager* gameManager)override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw(GameManager* gameManager)override;

private:


	SoundData soundData_ = {};

	Transform transformSprite_ = {};
	Transform transformSprite2_ = {};
	Transform transformSprite3_ = {};

	SpritePosition spriteAllPosition_ = {};
	SpritePosition spriteAllPosition2_ = {};
	SpritePosition spriteAllPosition3_ = {};

	int sampleTimer_ = 0;


	float transparency1 = 1.0f;
	float transparency2 = 1.0f;


};





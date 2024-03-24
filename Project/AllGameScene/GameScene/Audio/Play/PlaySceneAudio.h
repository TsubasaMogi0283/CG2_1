#pragma once
#include "IGameSceneAudio.h"

#include "Audio.h"

//前方宣言
class GameScene;

class PlaySceneAudio : public IGameSceneAudio {
public:
	//コンストラクタ
	PlaySceneAudio();


	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameScene* gameManager)override;



	/// デストラクタ
	~PlaySceneAudio();

private:
	Audio* bgm_ = nullptr;
	uint32_t bgmHandle_ = 0u;




};


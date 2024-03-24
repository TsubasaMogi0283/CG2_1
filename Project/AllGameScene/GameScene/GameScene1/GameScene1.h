#pragma once

#include "AllGameScene/GameManager/IGameScene.h"
#include <Player/Player.h>
#include "IGameScene1.h"
#include <Skydome/Skydome.h>
#include <memory>

class GameManager;

class GameScene1 : public IGameScene {
public:
	//コンストラクタ
	GameScene1();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;

	/// デストラクタ
	~GameScene1();


private:
	//共通部分はここでやる
	std::unique_ptr<Skydome> skydome_ = nullptr;


	Player* player_ = nullptr;
	
	int sceneNo_ = 0;
	int startTime_ = 0;


	IGameScene1* currentGamaScene1_ = nullptr;

};


#pragma once

#include "IGameScene1.h"


class GameScene1;

class GameScene1Ready:public IGameScene1 {

public:
	//コンストラクタ
	GameScene1Ready();


	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameScene1* gameManager)override;

	/// 描画
	void Draw()override;


	/// デストラクタ
	~GameScene1Ready();




};


#pragma once
#include "IGameScene.h"

class GameManager;

class TitleScene : public IGameScene {
public:

	//コンストラクタ
	TitleScene();

	/// デストラクタ
	~TitleScene();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;

private:


};


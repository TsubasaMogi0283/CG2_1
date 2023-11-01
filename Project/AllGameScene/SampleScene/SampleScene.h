#pragma once
#include "AllGameScene/GameManager/IGameScene.h"

#include "Polygon/Sprite/Sprite.h"
#include <Math/Vector/SpritePosition.h>
#include "Math/Vector/Transform.h"
#include "Camera/Camera.h"

#include "Polygon/Model/Model.h"
#include <DebugCamera/DebugCamera.h>
#include "Input/Input.h"

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
	DebugCamera* debugCamera_ = nullptr;
	Input* input_ = nullptr;

	Model* model_ = nullptr;
	Transform modelTransform_ = {};

	bool isActivateCamera_ = false;

};





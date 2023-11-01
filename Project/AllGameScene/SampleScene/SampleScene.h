#pragma once
#include "AllGameScene/GameManager/IGameScene.h"

#include "Polygon/Sprite/Sprite.h"
#include <Math/Vector/SpritePosition.h>
#include "Math/Vector/Transform.h"

#include <Player/Player.h>

#include "AllGameScene/GameManager/GameManager.h"
#include "Polygon/Model/Model.h"
#include "TextureManager/TextureManager.h"
#include <Enemy/Enemy.h>

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
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;


	Vector3 cameraTranslate_ = {};

	


};
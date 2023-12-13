#pragma once
#include "AllGameScene/GameManager/IGameScene.h"

#include "Sprite.h"
#include "SpritePosition.h"
#include "Transform.h"
#include "Model.h"


#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Enemy/Bullet/EnemyBullet.h"
#include "Enemy/Bullet/EnemyBullet.h"
#include "Collider/Collider.h"
#include "Collider/CollisionManager.h"
#include "Skydome/Skydome.h"
#include "RailCamera/RailCamera.h"

#include <memory>

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene();

	/// デストラクタ
	~SampleScene();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;

private:
	//衝突判定と応答
	void CheckAllCollisions();


private:
	Player* player_ = nullptr;
	static const int amount_ = 30;

	Enemy* enemy_[amount_] = { nullptr };
	std::unique_ptr<Skydome> skydome_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	Vector3 cameraTranslate_ = {};
	Vector3 cameraRotate_ = {};
	

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

};
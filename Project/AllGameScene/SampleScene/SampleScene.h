#pragma once
#include "AllGameScene/GameManager/IGameScene.h"

#include "Sprite.h"
#include "GameManager.h"
#include "Model.h"
#include "TextureManager.h"
#include "Camera.h"

#include <Object/Player/Player.h>
#include "Object/Player/Bullet/PlayerBullet.h"
#include <Object/Enemy/Enemy.h>
#include "Object/Enemy/Bullet/EnemyBullet.h"
#include <Object/Skydome/Skydome.h>
#include <Object/Collider/CollisionManager.h>
#include <Object/RailCamera/RailCamera.h>

#include <memory>
#include <list>
#include <Line/Line.h>

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

public:
	void AddPlayerBullet(PlayerBullet* playerBullet);
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void GenerateEnemy(Vector3 position);

private:
	//衝突判定と応答
	void CheckAllCollisions();


private:
	Player* player_ = nullptr;
	std::list<PlayerBullet*>playerBullets_;

	
	std::list<Enemy*> enemyes_;
	std::list<EnemyBullet*>enemyBullets_;

	std::unique_ptr<Skydome> skydome_ = nullptr;

	Vector3 cameraTranslate_ = {};
	Vector3 cameraRotate_ = {};


	std::unique_ptr < CollisionManager> collisionManager_ = nullptr;

	

	Line* lineSample_ = { nullptr };

	//カメラ
	Camera camera_ = {};
	RailCamera* railCamera_ = nullptr;
};


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

#include <sstream>
#include <Object/Ground/Ground.h>
#include <Object/FollowCamera/FollowCamera.h>

//StatePatternを使う時は必ず前方宣言をするように
class Gamemanager;

class SampleScene : public IGameScene {
public:

	//コンストラクタ
	SampleScene();

	/// 初期化
	void Initialize()override;

	/// 更新
	void Update(GameManager* gameManager)override;

	/// 描画
	void Draw()override;

	/// デストラクタ
	~SampleScene();

private:
	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;


	Camera camera_ = {};

};


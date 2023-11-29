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
#include "Enemy/Bullet/EnemyBullet.h"
#include "Collider/Collider.h"

//StatePatternを使う時は必ず前方宣言をするように
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
	//衝突判定と応答
	void CheckAllCollisions();

	//コライダー2つの衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	bool CheckBallCollosion(Vector3 v1, float v1Radious, Vector3 v2, float v2Radious);
private:
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;


	Vector3 cameraTranslate_ = {};
	Vector3 cameraRotate_ = {};
	

};
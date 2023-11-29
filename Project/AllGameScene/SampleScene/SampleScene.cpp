#include "SampleScene.h"

#include <list>


/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {
	player_ = new Player();
	player_->Initialize();



	enemy_ = new Enemy();
	enemy_->SetPlayer(player_);
	enemy_->Initialize();
	

	uint32_t textureHandle_ = TextureManager::LoadTexture("Resources/uvChecker.png");
	cameraTranslate_ = {0.0f,20.0f,-40.0f};
	cameraRotate_ = {0.4f,0.0f,0.0f};
	
}

bool SampleScene::CheckBallCollosion(Vector3 v1, float v1Radious, Vector3 v2, float v2Radious) {

	float x = (v2.x - v1.x);
	float y = (v2.y - v1.y);
	float z = (v2.z - v1.z);

	float resultPos = (x* x) + (y* y) + (z* z);
	
	float resultRadious = v1Radious + v2Radious;

	bool Flag = false;



	if (resultPos<=(resultRadious*resultRadious))
	{
		Flag = true;
	}

	return Flag;
}

//コライダー2つの衝突判定と応答
void SampleScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	//コライダーAのワールド座標を取得
	Vector3 colliderPosA = colliderA->GetWorldPosition();
	
	//コライダーBのワールド座標を取得
	Vector3 colliderPosB = colliderB->GetWorldPosition();
	
	//衝突フィルタリング
	//ビット演算だから&で
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}

	bool isHit=CheckBallCollosion(colliderPosA, colliderA->GetRadius(), colliderPosB, colliderB->GetRadius());

	if (isHit) 
	{
		colliderA->OnCollision();
		colliderB->OnCollision();
	}

}


/// <summary>
/// 衝突判定と応答
/// </summary>
void SampleScene::CheckAllCollisions(){
	//判定対象AとBの座標
	//資料ではpoAとかやっていたけど分かりずらいから具体的は変数名にする
	Vector3 playerPos = {};
	Vector3 enemyPos = {};
	Vector3 enemyBulletPos = {};
	Vector3 playerBulletPos = {};

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	//コライダー
	std::list<Collider*> colliders;
	//コライダーをリストに登録
	colliders.push_back(player_);
	colliders.push_back(enemy_);
	//自弾全てについて
	for (PlayerBullet* bullet : playerBullets) {
		colliders.push_back(bullet);
	}
	//敵弾全てについて
	for (EnemyBullet* bullet : enemyBullets) {
		colliders.push_back(bullet);
	}

	//総当たりの判定
	//総当たりだからそのままだと自機と自弾にも当たり判定が対応しちゃうよ
	std::list<Collider*>::iterator itrA = colliders.begin();
	for (; itrA != colliders.end(); ++itrA) {
		
		//itrAの*は「参照」
		//itrAを全通りcolliderAに登録しているよ
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders.end(); ++itrB) {
			Collider* colliderB = *itrB;

			CheckCollisionPair(colliderA, colliderB);

		}

	}



}




/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	Camera::GetInstance()->Camera::SetTranslate(cameraTranslate_);
	Camera::GetInstance()->Camera::SetRotate(cameraRotate_);


	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &cameraTranslate_.x, -20.0f, 10.0f);
	ImGui::SliderFloat3("Rotate", &cameraRotate_.x, -7.0f, 7.0f);

	ImGui::End();

	//当たり判定
	CheckAllCollisions();

	player_->Update();
	
	enemy_->Update();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	player_->Draw();

	enemy_->Draw();
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete player_;
	delete enemy_;
}

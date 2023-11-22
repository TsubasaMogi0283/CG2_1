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

#pragma region 自キャラと敵弾の当たり判定
	//自キャラのワールド座標
	playerPos = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//敵弾のワールド座標
		enemyBulletPos = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distanceAB = Length(Subtract(playerPos, enemyBulletPos));
		if (distanceAB < player_->GetRadius() + bullet->GetRadius()) {
			player_->OnCollision();
			bullet->OnCollision();
		}

	}

#pragma endregion


#pragma region 自弾と敵キャラの当たり判定
	//敵の座標
	enemyPos = enemy_->GetWorldPosition();
	//自弾と敵キャラ全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		//敵弾のワールド座標
		playerBulletPos = bullet->GetWorldPosition();

		//距離を求める
		float distanceCD = Length(Subtract(playerBulletPos,enemyPos));
		if (distanceCD < enemy_->GetRadius() + bullet->GetRadius()) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}

	}



#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自弾と敵弾全ての当たり判定
	//二重for文総当たりで
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			//敵弾のワールド座標
			playerBulletPos = playerBullet->GetWorldPosition();
			//敵弾のワールド座標
			enemyBulletPos = enemyBullet->GetWorldPosition();
			
			//距離を求める
			float distance = Length(Subtract(playerBulletPos,enemyBulletPos));
			if (distance < playerBullet->GetRadius() + enemyBullet->GetRadius()) {
				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		
		}
	}
#pragma endregion


}

//コライダー2つの衝突判定と応答
void SampleScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	//コライダーAのワールド座標を取得
	Vector3 colliderPosA = colliderA->GetWorldPosition();
	
	//コライダーBのワールド座標を取得
	Vector3 colliderPosB = colliderB->GetWorldPosition();
	
	float distance = Length(Subtract(colliderPosA, colliderPosB));

	//球と球の交差判定
	if (distance < colliderA->GetRadius() + colliderB->GetRadius()) {
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();

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

#include "SampleScene.h"




/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}

//敵発生用関数
void SampleScene::GenerateEnemy(Vector3 position) {
	enemy_ = new Enemy();
	enemy_->SetPlayer(player_);
	enemy_->Initialize();
	enemy_->SetGameScene(this);

	enemyes_.push_back(enemy_);
}

/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {
	player_ = new Player();
	Vector3 playerPosition = { 0.0f, 0.0f, 30.0f };
	player_->Initialize(playerPosition);

	/*enemy_ = new Enemy();
	enemy_->SetPlayer(player_);
	enemy_->Initialize();
	enemy_->SetGameScene(this);*/

	GenerateEnemy

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	collisionManager_ = std::make_unique < CollisionManager>();

	railCamera_ = new RailCamera();
	railCamera_->Initialize(player_->GetWorldPosition(), { 0.0f,0.0f,0.0f });
	player_->SetParent(&railCamera_->GetWorldTransform());

	cameraTranslate_ = { 0.0f,20.0f,-40.0f };
	cameraRotate_ = { 0.4f,0.0f,0.0f };
	camera_.Initialize();
	camera_.translate_ = cameraTranslate_;
	camera_.rotate_ = cameraRotate_;
}





/// <summary>
/// 衝突判定と応答
/// </summary>
void SampleScene::CheckAllCollisions() {
	//判定対象AとBの座標
	//資料ではpoAとかやっていたけど分かりずらいから具体的は変数名にする
	Vector3 playerPos = {};
	Vector3 enemyPos = {};
	Vector3 enemyBulletPos = {};
	Vector3 playerBulletPos = {};

	//自弾リストの取得
	//const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	//コライダー
	std::list<Collider*> colliders;



	//衝突マネージャのリストをクリアする
	collisionManager_->ClearList();
	//コライダーを全て衝突マネージャに登録する
	collisionManager_->RegisterList(player_);
	collisionManager_->RegisterList(enemy_);


	//自弾全てについて
	for (PlayerBullet* bullet : playerBullets_) {
		//colliders.push_back(bullet);
		collisionManager_->RegisterList(bullet);
	}
	//敵弾全てについて
	for (EnemyBullet* bullet : enemyBullets_) {
		//colliders.push_back(bullet);
		collisionManager_->RegisterList(bullet);
	}

	collisionManager_->CheckAllCollision();





}

//Playerの弾を追加
void SampleScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	if (playerBullet != nullptr) {
		playerBullets_.push_back(playerBullet);
	}
	else {
		ImGui::Begin("Error");
		ImGui::End();
	}
}

//Enemyの弾を追加
void SampleScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//enemyBullets_.push_back(enemyBullet);

}


/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {


#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &camera_.translate_.x, -40.0f, 40.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -7.0f, 7.0f);

	ImGui::End();

#endif




	//当たり判定
	CheckAllCollisions();

	player_->Update();
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}
	//デスフラグの立った玉を削除
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});
	
#pragma region Enemy

	enemy_->Update();
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}
	enemyBullets_.remove_if([](EnemyBullet* newEnemyBullets) {
		if (newEnemyBullets->IsDead()) {
			delete newEnemyBullets;
			return true;
		}
		return false;
		});

#pragma endregion

	skydome_->Update();





	//カメラ

	railCamera_->Update();
	camera_.viewMatrix_ = railCamera_->GetViewProjection().viewMatrix_;
	camera_.projectionMatrix_ = railCamera_->GetViewProjection().projectionMatrix_;

	camera_.Transfer();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {

	skydome_->Draw(camera_);
	player_->Draw(camera_);
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(camera_);
	}

	enemy_->Draw(camera_);
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Draw(camera_);
	}

}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {

	delete player_;
	/*for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}*/

	delete enemy_;
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
	}

	delete railCamera_;
}

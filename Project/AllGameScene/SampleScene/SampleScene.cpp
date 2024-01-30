#include "SampleScene.h"

#include <list>
#include <VectorCalculation.h>


/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}

void SampleScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	playerBullets_.push_back(playerBullet);
}

void SampleScene::AddEnemyBullet(EnemyBullet* enemyBullet){
	enemyBullets_.push_back(enemyBullet);
}


/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {
	player_ = new Player();
	Vector3 playerPosition = { 0.0f, 0.0f, 30.0f };

	//前方宣言したものは初期化より先に置いてね
	//そうじゃないとリストでエラーが起きるよ
	player_->SetSampleScene(this);
	player_->Initialize(playerPosition);

	enemy_ = new Enemy();
	enemy_->SetPlayer(player_);
	enemy_->SetSampleScene(this);
	enemy_->Initialize();


	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	collisionManager_ = std::make_unique < CollisionManager>();

	camera_.Initialize();

	railCamera_ = new RailCamera();
	railCamera_->Initialize(camera_,player_->GetWorldPosition(), { 0.0f,0.0f,0.0f });
	player_->SetParent(&railCamera_->GetWorldTransform());

	
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




/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {



	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &camera_.translate_.x, -40.0f, 40.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -7.0f, 7.0f);

	ImGui::End();


	//当たり判定
	CheckAllCollisions();

	//先にビュー行列から計算する
	railCamera_->Update();
	camera_.viewMatrix_ = railCamera_->GetViewProjection().viewMatrix_;
	camera_.projectionMatrix_ = railCamera_->GetViewProjection().projectionMatrix_;


#pragma region プレイヤー

	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->Update();
	

	//デスフラグの立った玉を削除
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}

#pragma endregion


	enemy_->Update();


	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();

	}

	//デスフラグの立った玉を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	
	skydome_->Update();
	


	
	camera_.Transfer();


	//camera_.viewMatrix_ = railCamera_->GetViewProjection().viewMatrix_;
	//camera_.projectionMatrix_ = railCamera_->GetViewProjection().projectionMatrix_;
	//camera_.Update();
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
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(camera_);

	}
	//線分の数
	
	railCamera_->Draw(camera_);

}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {

	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
	delete player_;
	

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;

	}
	delete enemy_;
	delete railCamera_;

	

	delete lineSample_;
}

#include "SampleScene.h"
#include "GameManager.h"
#include <list>
#include "TextureManager.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {

	player_ = new Player();
	player_->Initialize();

	Vector3 radian = { 0.0f,0.0f,0.0f };
	railCamera_ = new RailCamera();
	//railCamera_->Initialize(player_->GetWorldPosition(), radian);

	//player_->SetParent(&railCamera_->GetWorldmatrix());

	for (int i = 0; i < amount_; i++) {
		enemy_[i] =new Enemy();
		enemy_[i]->SetPlayer(player_);
		enemy_[i]->Initialize();
	
	}
	

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();


	collisionManager_ = std::make_unique <CollisionManager>();

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
	for (int i = 0; i < amount_; i++) {
		const std::list<EnemyBullet*>& enemyBullets = enemy_[i]->GetBullets();

		//コライダー
		std::list<Collider*> colliders;



		//衝突マネージャのリストをクリアする
		collisionManager_->ClearList();
		//コライダーを全て衝突マネージャに登録する
		collisionManager_->RegisterList(player_);
		for (int i = 0; i < amount_; i++) {
			collisionManager_->RegisterList(enemy_[i]);
		}

		//自弾全てについて
		for (PlayerBullet* bullet : playerBullets) {
			//colliders.push_back(bullet);
			collisionManager_->RegisterList(bullet);
		}
		//敵弾全てについて
		for (EnemyBullet* bullet : enemyBullets) {
			//colliders.push_back(bullet);
			collisionManager_->RegisterList(bullet);
		}

		collisionManager_->CheckAllCollision();
	}
	
	//衝突マネージャのリストをクリアする
	collisionManager_->ClearList();
}




/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	Camera::GetInstance()->SetTranslate(cameraTranslate_);
	Camera::GetInstance()->SetRotate(cameraRotate_);

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &cameraTranslate_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("Rotate", &cameraRotate_.x, -7.0f, 7.0f);
	ImGui::End();

	//当たり判定
	CheckAllCollisions();

	player_->Update();
	
	for (int i = 0; i < amount_; i++) {
		enemy_[i]->Update();
	}
	skydome_->Update();

	//railCamera_->Update();


}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	
	skydome_->Draw();
	player_->Draw();

	for (int i = 0; i < amount_; i++) {
		enemy_[i]->Draw();
	}

}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete player_;
	for (int i = 0; i < amount_; i++) {
		delete enemy_[i];
	}
	delete railCamera_;

}

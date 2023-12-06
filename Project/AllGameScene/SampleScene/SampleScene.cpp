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
	

	skydome_ = new Skydome();
	skydome_->Initialize();

	collisionManager_ = new CollisionManager();

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

	//コライダー
	std::list<Collider*> colliders;



	//衝突マネージャのリストをクリアする
	collisionManager_->ClearList();
	//コライダーを全て衝突マネージャに登録する
	collisionManager_->RegisterList(player_);
	collisionManager_->RegisterList(enemy_);


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

	skydome_->Update();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	
	skydome_->Draw();
	player_->Draw();

	enemy_->Draw();


}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete player_;
	delete enemy_;

	delete skydome_;
	delete collisionManager_;
}

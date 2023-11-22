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
	cameraTranslate_ = Camera::GetInstance()->GetTranslate();

	
}

/// <summary>
/// 衝突判定と応答
/// </summary>
void SampleScene::CheckAllCollisions(){
	//判定対象AとBの座標
	Vector3 posA = {};
	Vector3 posB = {};

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラのワールド座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//敵弾のワールド座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離を求める
		float distanceAB = Length(Subtract(posA, posB));
		if (distanceAB < player_->GetRadius() + bullet->GetRadius()) {
			player_->OnCollision();
			bullet->OnCollision();
		}

		ImGui::Begin("Collision");
		ImGui::InputFloat("Distance", &distanceAB);
		ImGui::End();
	}

#pragma endregion


#pragma region 自弾と敵キャラの当たり判定

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

#pragma endregion


}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	Camera::GetInstance()->Camera::SetTranslate(cameraTranslate_);


	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &cameraTranslate_.x, -20.0f, 10.0f);

	ImGui::End();

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

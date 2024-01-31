#include "SampleScene.h"

#include <list>
#include <VectorCalculation.h>


/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}


/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {
	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize();

	//天球
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	//天球
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	//追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTraget(&player_->GetWorldTransform());

	camera_.Initialize();
	
}





/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {



	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &camera_.translate_.x, -40.0f, 40.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -7.0f, 7.0f);

	ImGui::End();


	//天球
	skydome_->Update();

	//地面
	ground_->Update();

	//プレイヤー
	player_->Update(camera_);
	

	//追従カメラ
	//「現在」の座標を参照したいのでfollowCameraは後かな
	followCamera_->Update();

	//追従カメラクラスで計算したものをコピーしてくる
	camera_.viewMatrix_ = followCamera_->GetCamera().viewMatrix_;
	camera_.projectionMatrix_ = followCamera_->GetCamera().projectionMatrix_;
	
	//転送
	camera_.Transfer();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {

	//プレイヤー
	player_->Draw(camera_);
	
	//天球
	skydome_->Draw(camera_);
	
	//地面
	ground_->Draw(camera_);
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {

	

}

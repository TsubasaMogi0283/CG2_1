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
	player_ = std::make_unique<Player>();
	Vector3 playerPosition = { 0.0f, 0.0f, 30.0f };

	player_->Initialize(playerPosition);


	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize();

	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-20.0f };
	
}





/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {



	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &camera_.translate_.x, -40.0f, 40.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -7.0f, 7.0f);

	ImGui::End();


	skydome_->Update();
	player_->Update(camera_);
	




	//camera_.viewMatrix_ = railCamera_->GetViewProjection().viewMatrix_;
	//camera_.projectionMatrix_ = railCamera_->GetViewProjection().projectionMatrix_;
	camera_.Update();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {

	player_->Draw(camera_);
	skydome_->Draw(camera_);

}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {

	

}

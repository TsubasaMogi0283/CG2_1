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
void SampleScene::Initialize() {
	player_ = new Player();
	Vector3 playerPosition = { 0.0f, 0.0f, 0.0f };
	player_->Initialize(playerPosition);


	camera_.Initialize();
	camera_.translate_ = {0.0f, 0.0f, -20.0f};
	camera_.rotate_ = cameraRotate_;
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {



	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &camera_.translate_.x, -40.0f, 40.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -7.0f, 7.0f);

	ImGui::End();


	player_->Update();


	camera_.Update();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {

	player_->Draw(camera_);


	
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {

	delete player_;
}

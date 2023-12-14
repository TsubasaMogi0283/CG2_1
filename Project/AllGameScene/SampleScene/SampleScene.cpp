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
	//CheckAllCollisions();

	player_->Update();


}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	
	player_->Draw();

}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete player_;

}

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

	particle3D_ = new Particle3D();
	particle3D_->Create("Resources/05_02", "plane.obj");
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");

	line_ = new Line();
	line_->Initialize();

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

	particle3D_->Update(camera_);
	camera_.Update();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {

	//player_->Draw(camera_);
	//line_->Draw({ 0.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f },camera_);
	particle3D_->Draw(camera_, textureHandle_);
	
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete line_;
	delete player_;
}

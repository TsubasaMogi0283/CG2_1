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

	sphere = std::make_unique<Model>();
	sphere->CreateObj("Resources/CG3/Sphere", "Sphere.obj");


	sphereWorldTransform_.Initialize();
	const float SCALE = 3.0f;
	sphereWorldTransform_.scale_ = { SCALE,SCALE,SCALE };


	
	terrain_ = std::make_unique<Model>();
	terrain_->CreateObj("Resources/CG3/terrain", "terrain.obj");


	terrainWorldTransform_.Initialize();

	particle3D_ = new Particle3D();
	particle3D_->Create("Resources/05_02", "plane.obj");
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");


	camera_.Initialize();
	camera_.translate_ = {0.0f, 15.0f, -20.0f};
	camera_.rotate_ = { -5.75f,0.0f,0.0f };
	
}






/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {


	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Tranlate", &camera_.translate_.x, -40.0f, 40.0f);
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -7.0f, 7.0f);
	ImGui::SliderFloat("Intensity", &pointLightIntensity_, 0.0f, 1.0f);
	ImGui::SliderFloat3("Position", &pointLightPosition_.x, -10.0f, 10.0f);

	ImGui::End();

	sphere->SetPointLightPosition(pointLightPosition_);
	sphere->SetPointLightIntensity(pointLightIntensity_);
	
	terrain_->SetPointLightPosition(pointLightPosition_);
	terrain_->SetPointLightIntensity(pointLightIntensity_);



	/*particle3D_->SetCameraAffineMatrix(camera_.worldMatrix_);
	particle3D_->SetViewMatrix(camera_.viewMatrix_);
	particle3D_->SetProjectionMatrix(camera_.projectionMatrix_);
	particle3D_->Update();*/
	sphereWorldTransform_.Update();
	terrainWorldTransform_.Update();
	camera_.Update();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	terrain_->Draw(terrainWorldTransform_, camera_);
	//sphere->Draw(sphereWorldTransform_,camera_);
	//particle3D_->Draw(textureHandle_);
	
}




/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete particle3D_;
}

#include "SampleScene.h"
#include "GameManager.h"
#include "TextureManager.h"
#include <imgui.h>
#include "Camera.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {
	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		model_[i] = Model::Create("Resources/CG3/fence", "fence.obj");
	}
	modelWorldTransform_.Initialize();
	modelWorldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	

	sprite = std::make_unique<Sprite>();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/uvChecker.png");
	spritePosition_ = { 100.0f,100.0f };
	sprite.reset(Sprite::Create(textureHandle, spritePosition_));

	

	//particle_ = std::make_unique<Particle3D>();
	//particle_->Create("Resources/05_02", "plane.obj");
	int count = 3;
	


	//0.5秒ごとに発生
	float frequency = 0.5f;
	//発生頻度用の時刻。0.0で初期化
	float frequencyTime = 0.0f;



	particleTextureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");

	/*accelerationField_.acceleration = { 5.0f,0.0f,0.0, };
	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField_.area.max = { 1.0f,1.0f,1.0f };

	isSetField_ = true;
	particle_->SetField(isSetField_);
	particle_->SetAccelerationField(accelerationField_);

	particle_->SetCount(count);
	particle_->SetFrequency(frequency);
	particle_->SetFrequencyTime(frequencyTime);
	particleTranslate_ = { 0.0f,0.0f,0.0f };*/


	camera_.Initialize();
	camera_.translate_ = { 0.0f,0.0f,-9.8f };

	audio_ = Audio::GetInstance();
	audioHandle_ = Audio::LoadWave("Resources/Audio/Sample/Win.wav");
	audio_->PlayWave(audioHandle_, true);

	audio2_ = Audio::GetInstance();;
	audioHandle2_ = Audio::LoadWave("Resources/Audio/Sample/Win.wav");
	audio2_->PlayWave(audioHandle2_, false);

	cameraRotate_ = {};
	//Camera::GetInstance()->SetRotate(cameraRotate_);
}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translate", &camera_.translate_.x, -20.0f, 20.0f);
	ImGui::End();
	modelWorldTransform_.Update();
	camera_.Update();

	model_[0]->SetColor(modelColor_);
	//model_[0]->SetTranslate(modelTranslate_);

	/*particle_->SetTranslate(particleTranslate_);
	particle_->SetField(isSetField_);
	particle_->Update();*/

	/*ImGui::Begin("Plane");
	ImGui::SliderFloat3("Translate", &modelTranslate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat4("Color", &modelColor_.x, 0.0f, 1.0f);
	ImGui::Checkbox("isSetField;", &isSetField_);

	ImGui::End();
	

	ImGui::Begin("Particle");
	ImGui::SliderFloat3("Translate", &particleTranslate_.x, -3.0f, 3.0f);
	ImGui::End();
	*/

	sprite->SetPosition(spritePosition_);

	//ウィンドウサイズの設定は↓でやるよ
#ifdef _DEBUG
	ImGui::SetNextWindowSize(ImVec2(500, 100));
	ImGui::Begin("Sprite");
	ImGui::SliderFloat2("Position", &spritePosition_.x, 0.0f, 500.0f,"%.1f");
	ImGui::End();

#endif

}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		model_[i]->Draw(modelWorldTransform_,camera_);
	
	}
	//particle_->Draw(particleTextureHandle_);
	sprite->Draw();
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		delete model_[i];
	}
}

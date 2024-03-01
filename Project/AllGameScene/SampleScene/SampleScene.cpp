#include "SampleScene.h"
#include "GameManager.h"
#include "TextureManager.h"
#include <imgui.h>
#include <Input.h>
#include <Camera.h>


#include "SampleScene2/SampleScene2.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize() {

	uint32_t modelData = ModelManager::GetInstance()->LoadObject("Resources/CG3/terrain", "terrain.obj");


	uint32_t model1 = ModelManager::GetInstance()->LoadObject("Resources/CG3/terrain", "terrain.obj");
	uint32_t model2 = ModelManager::GetInstance()->LoadObject("Resources/CG3/Sphere", "Sphere.obj");

	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		model_[0].reset(Model::Create(model1));
		model_[1].reset(Model::Create(model2));
		modelWorldTransform_[i].Initialize();
	}

	const float SCALE_SIZE = 1.0f;
	modelWorldTransform_[0].scale_ = modelScale_;
	modelWorldTransform_[1].scale_ = modelScale_;



	modelWorldTransform_[1].translate_ = { 0.0f,-1.0f,0.0f };


	sprite = std::make_unique<Sprite>();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/uvChecker.png");
	spritePosition_ = { 100.0f,100.0f };
	sprite.reset(Sprite::Create(textureHandle, spritePosition_));

	uint32_t textureHandle2 = TextureManager::LoadTexture("Resources/uvChecker.png");
	
	uint32_t textureHandle3 = TextureManager::LoadTexture("Resources/monsterBall.png");
	uint32_t textureHandle4 = TextureManager::LoadTexture("Resources/bullet.png");

	//uint32_t particleModel= ModelManager::GetInstance()->LoadObject("Resources/05_02", "plane.obj");
	//particle_.reset(Particle3D::Create(particleModel));
	//int count = 3;
	



	//0.5秒ごとに発生
	float frequency = 0.5f;
	//発生頻度用の時刻。0.0で初期化
	float frequencyTime = 0.0f;



	particleTextureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");

	//accelerationField_.acceleration = { 5.0f,0.0f,0.0, };
	//accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	//accelerationField_.area.max = { 1.0f,1.0f,1.0f };
	//
	//isSetField_ = true;
	//particle_->SetField(isSetField_);
	//particle_->SetAccelerationField(accelerationField_);
	//
	//particle_->SetCount(count);
	//particle_->SetFrequency(frequency);
	//particle_->SetFrequencyTime(frequencyTime);
	//particleTranslate_ = { 0.0f,0.0f,0.0f };
	//

	cameraTranslate_ = { 0.0f,10.0f,-30.0f };
	camera_.Initialize();
	camera_.rotate_ = { 0.2f,0.0f,0.0f };
	camera_.translate_ = cameraTranslate_;




	audio_ = Audio::GetInstance();
	uint32_t audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/Win.wav");
	//audio_->PlayWave(audioHandle_, true);

}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {


	model_[0]->SetColor(modelColor_);
	//model_[0]->SetDirectionalLightIntensity(intensity_);
	//model_[1]->SetDirectionalLightIntensity(intensity_);
	//model_[0]->SetDirectionalLightDirection(lightingDirection_);
	//model_[1]->SetDirectionalLightDirection(lightingDirection_);




	const float SPEED = 1.0f;
	

	/*pointLightPosition_.x += move.x;
	pointLightPosition_.z += move.y;
	if (pointLightPosition_.x > 15.0f ||
		pointLightPosition_.x < -15.0f){
		move.x *= -1.0f;
	}
	if (pointLightPosition_.z > 10.0f ||
		pointLightPosition_.z < -10.0f) {
		move.y *= -1.0f;
	}*/


	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		model_[i]->SetSpotLightPosition(spotLightPosition_);
		model_[i]->SetSpotLightIntensity(spotLightIntensity_);
		model_[i]->SetSpotLightColor(spotLightColor_);
		model_[i]->SetSpotLightDecay(spotLightDecay_);
		model_[i]->SetSpotLightDistance(spotLightDistance_);
		model_[i]->SetSpotLightDirection(spotLightDirection_);
		model_[i]->SetSpotLightCosAngle(cosAngle_);
		model_[i]->SetCosFalloffStart(cosFalloffStart_);

	}



	//particle_->SetTranslate(particleTranslate_);

	//particle_->SetField(isSetField_);


	sprite->SetPosition(spritePosition_);

	const float MOVE_AMOUNT=1.0f;


	const float CAMERA_VELOCITY = 0.05f;
	if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		camera_.translate_.x += MOVE_AMOUNT * CAMERA_VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		camera_.translate_.x -= MOVE_AMOUNT * CAMERA_VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		camera_.translate_.y += MOVE_AMOUNT * CAMERA_VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		camera_.translate_.y -= MOVE_AMOUNT * CAMERA_VELOCITY;
	}


	
	//ウィンドウサイズの設定は↓でやるよ
#ifdef _DEBUG
	ImGui::Begin("Model");
	ImGui::SliderFloat("Intensity", &spotLightIntensity_, 0.0f, 3.0f);
	ImGui::SliderFloat3("spotLightPosition_", &spotLightPosition_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("spotLightDirection_",&spotLightDirection_.x,-1.0f,1.0f);
	ImGui::SliderFloat("spotLightDecay_",&spotLightDecay_,0.0f,30.0f);
	ImGui::SliderFloat("spotLightDistance_", &spotLightDistance_, 0.0f, 30.0f);
	ImGui::SliderFloat("cosFalloffStart_", &cosFalloffStart_, 0.0f, 1.0f);
	ImGui::SliderFloat("cosAngle_", &cosAngle_, 0.0f, 1.0f);





	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -40.0f, 40.0f);
	ImGui::End();


	ImGui::SetNextWindowSize(ImVec2(500, 100));
	ImGui::Begin("Sprite");
	ImGui::SliderFloat2("Position", &spritePosition_.x, 0.0f, 500.0f, "%.1f");
	ImGui::End();

#endif

	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		modelWorldTransform_[i].Update();
	}
	camera_.Update();



	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		
		audio_->StopWave(audioHandle2_);
		isEnd_ = true;
	}

	if (isEnd_ == true) {
		audio_->StopWave(audioHandle2_);
		gameManager->ChangeScene(new SampleScene2());
	}



}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		model_[i]->Draw(modelWorldTransform_[i], camera_);
	
	}
	//particle_->Draw(particleTextureHandle_,camera_);
	sprite->Draw();
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
}

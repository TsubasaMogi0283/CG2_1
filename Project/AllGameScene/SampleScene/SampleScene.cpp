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
	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		model_[i] = Model::Create("Resources/CG3/fence", "fence.obj");
	}
	modelTranslate_ = {0.0f,0.0f,0.0f};

	sprite = std::make_unique<Sprite>();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/uvChecker.png");
	spritePosition_ = { 100.0f,100.0f };
	sprite.reset(Sprite::Create(textureHandle, spritePosition_));

	

	particle_ = std::make_unique<Particle3D>();
	particle_->Create("Resources/05_02", "plane.obj");
	int count = 3;
	



	//0.5秒ごとに発生
	float frequency = 0.5f;
	//発生頻度用の時刻。0.0で初期化
	float frequencyTime = 0.0f;



	particleTextureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");

	accelerationField_.acceleration = { 5.0f,0.0f,0.0, };
	accelerationField_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationField_.area.max = { 1.0f,1.0f,1.0f };

	isSetField_ = true;
	particle_->SetField(isSetField_);
	particle_->SetAccelerationField(accelerationField_);

	particle_->SetCount(count);
	particle_->SetFrequency(frequency);
	particle_->SetFrequencyTime(frequencyTime);
	particleTranslate_ = { 0.0f,0.0f,0.0f };

	cameraTranslate_ = { 0.0f,0.0f,-20.0f };
	Camera::GetInstance()->SetTranslate(cameraTranslate_);


	audio_ = Audio::GetInstance();
	uint32_t audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/Win.wav");
	audio_->PlayWave(audioHandle_, true);

}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {


	model_[0]->SetColor(modelColor_);
	model_[0]->SetTranslate(modelTranslate_);

	particle_->SetTranslate(particleTranslate_);

	particle_->SetField(isSetField_);
	particle_->Update();


	sprite->SetPosition(spritePosition_);

	const float MOVE_AMOUNT=1.0f;
	const float VELOCITY = 3.0f;
	if (Input::GetInstance()->IsPushKey(DIK_D) == true) {
		spritePosition_.x += MOVE_AMOUNT * VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_A) == true) {
		spritePosition_.x -= MOVE_AMOUNT * VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_W) == true) {
		spritePosition_.y -= MOVE_AMOUNT * VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_S) == true) {
		spritePosition_.y += MOVE_AMOUNT * VELOCITY;
	}


	const float CAMERA_VELOCITY = 0.05f;
	if (Input::GetInstance()->IsPushKey(DIK_RIGHT) == true) {
		cameraTranslate_.x += MOVE_AMOUNT * CAMERA_VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_LEFT) == true) {
		cameraTranslate_.x -= MOVE_AMOUNT * CAMERA_VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_UP) == true) {
		cameraTranslate_.y += MOVE_AMOUNT * CAMERA_VELOCITY;
	}
	if (Input::GetInstance()->IsPushKey(DIK_DOWN) == true) {
		cameraTranslate_.y -= MOVE_AMOUNT * CAMERA_VELOCITY;
	}

	Camera::GetInstance()->SetTranslate(cameraTranslate_);


	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		
		audio_->StopWave(audioHandle2_);
		isEnd_ = true;
	}

	if (isEnd_ == true) {
		audio_->StopWave(audioHandle2_);
		gameManager->ChangeScene(new SampleScene2());
	}


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
		model_[i]->Draw();
	
	}
	particle_->Draw(particleTextureHandle_);
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

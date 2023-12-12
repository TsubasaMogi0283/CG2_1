#include "SampleScene.h"
#include "GameManager.h"
#include "TextureManager.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {
	for (int i = 0; i < MODEL_AMOUNT_; i++) {
		model_[i] = Model::Create("Resources/CG3/fence", "fence.obj");
	}
	modelTranslate_ = {0.0f,0.0f,0.0f};

	sprite = std::make_unique<Sprite>();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/uvChecker.png");
	sprite.reset(Sprite::Create(textureHandle, { 0.0f,0.0f }));

	audio_ = Audio::GetInstance();
	uint32_t audioHandle_ = audio_->LoadWave("Resources/Audio/Sample/Win.wav");
	audio_->PlayWave(audioHandle_, true);


	particle_ = std::make_unique<Particle3D>();
	particle_->Create("Resources/05_02", "plane.obj");
	int count = 3;
	
	//0.5秒ごとに発生
	float frequency = 0.5f;
	//発生頻度用の時刻。0.0で初期化
	float frequencyTime = 0.0f;



	particleTextureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");

	particle_->SetCount(count);
	particle_->SetFrequency(frequency);
	particle_->SetFrequencyTime(frequencyTime);

}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {


	model_[0]->SetColor(modelColor_);
	model_[0]->SetTranslate(modelTranslate_);

	particle_->Update();

	ImGui::Begin("Plane");
	ImGui::SliderFloat3("Translate", &modelTranslate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat4("Color", &modelColor_.x, 0.0f, 1.0f);
	

	ImGui::End();
	

	


}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
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

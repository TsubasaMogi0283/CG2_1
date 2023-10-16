#include "SampleScene.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	sprite_->Release();
	audio_->SoundUnload(&soundData_);
	//delete sprite_;
	//delete sprite2_;
	//delete sprite3_;
	//plane_->Release();
	//delete plane_;

	audio_->DeleteInstance();;
}

/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {
	

	textureManager_ = TextureManager::GetInstance();
	
	 
	 
	 

	//TextureHandleはそれぞれ違う値になっているのに何故
	uint32_t textureHandle_ = TextureManager::LoadTexture("Resources/uvChecker.png");
	uint32_t textureHandle2_= TextureManager::LoadTexture("Resources/monsterBall.png");
	uint32_t textureHandle3_= TextureManager::LoadTexture("Resources/bullet.png");


	transformSprite_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	sprite_ = new Sprite();
	sprite_->LoadTextureHandle(textureHandle_);
	spriteAllPosition_ = { {0.0f,0.0f},{0.0f,512.0f},{512.0f,0.0f},{512.0f,512.0f} };
	sprite_->SetAllPosition(spriteAllPosition_);

	transformSprite2_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{300.0f,0.0f,0.0f} };
	
	sprite2_ = new Sprite();
	sprite2_->LoadTextureHandle(textureHandle2_);
	spriteAllPosition2_ = { {0.0f,0.0f},{0.0f,512.0f},{512.0f,0.0f},{512.0f,512.0f} };
	sprite2_->SetAllPosition(spriteAllPosition2_);

	

	transformSprite3_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{800.0f,0.0f,0.0f} };
	
	//sprite3_ = new Sprite();
	//sprite3_->LoadTextureHandle(textureHandle3_);
	//spriteAllPosition3_ = { {0.0f,0.0f},{0.0f,16.0f},{16.0f,0.0f},{16.0f,16.0f} };
	//sprite3_->SetAllPosition(spriteAllPosition3_);




	transformModel_ = {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}};
	//plane_ = new Model();
	///plane_->CreateObject("Resources/Sample", "enemy.obj");
	
	

	audio_ = Audio::GetInstance();
	audio_->Initialize();
	soundData_ = audio_->LoadWave("Resources/Audio/Sample/Hit.wav");


	audio_->PlayWave(soundData_ ,true);
	

	
}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	sampleTimer_ += 1;
	if (sampleTimer_ >30) {
		//audio_->StopWave(soundData_);
	}
	if (sampleTimer_ > 120) {
		//gameManager->ChangeScene(new SampleScene2());
	}
	
	sprite_->SetTransparency(transparency1);
	sprite2_->SetTransparency(transparency2);


	ImGui::Begin("SampleScene1");
	ImGui::Text("Time",sampleTimer_);
	ImGui::End();


	ImGui::Begin("Sprite");
	ImGui::InputFloat3("scale", &transformSprite_.scale.x);
	ImGui::SliderFloat3("scale", &transformSprite_.scale.x,0.0f,10.0f);
	
	ImGui::InputFloat("transparency", &transparency1);
	ImGui::SliderFloat("transparency", &transparency1, 0.0f, 1.0f);
	ImGui::End();

	ImGui::Begin("Sprite2");
	ImGui::InputFloat("transparency", &transparency2);
	ImGui::SliderFloat("transparency", &transparency2, 0.0f, 1.0f);
	ImGui::End();


	ImGui::Begin("Model");
	ImGui::InputFloat3("translate", &transformModel_.translate.x);
	ImGui::SliderFloat3("translate", &transformModel_.translate.x,-10.0f,10.0f);
	ImGui::End();
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	//plane_->Draw(transformModel_);
	sprite_->DrawRect(transformSprite_);
	sprite2_->DrawRect(transformSprite2_);
	//sprite3_->DrawRect(transformSprite3_);

}

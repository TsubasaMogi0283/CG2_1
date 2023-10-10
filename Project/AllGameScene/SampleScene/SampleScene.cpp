#include "SampleScene.h"
#include "AllGameScene/SampleScene2/SampleScene2.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	//sprite_->Release();
	audio_->SoundUnload(&soundData_);
	delete sprite_;
}

/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {
	sprite_ = new Sprite();
	//sprite_->Initialize();
	sprite_->LoadTexture("Resources/uvChecker.png");

	transformSprite_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	spriteAllPosition_ = { {0.0f,0.0f},{0.0f,512.0f},{512.0f,0.0f},{512.0f,512.0f} };
	sprite_->SetAllPosition(spriteAllPosition_);


	plane_ = new Model();
	plane_->CreateObject("Resources/05_02","/plane.obj");
	plane_->LoadTexture("Resources/05_02/plane.obj");

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
	if (sampleTimer_ > 60) {
		gameManager->ChangeScene(new SampleScene2());
	}
	
	ImGui::Begin("SampleScene1");
	ImGui::Text("Time",sampleTimer_);
	ImGui::End();

}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {

	//カメラ行列
	cameraMatrix_ = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	
	//遠視投影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(WINDOW_SIZE_WIDTH_) / float(WINDOW_SIZE_HEIGHT_), 0.1f, 100.0f);
			
	plane_->Draw(transformModel,viewMatrix,projectionMatrix);


	sprite_->DrawRect(transformSprite_);
}

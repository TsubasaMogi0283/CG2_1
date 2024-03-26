#include "SampleScene.h"
#include "GameManager.h"
#include "TextureManager.h"
#include <imgui.h>
#include <Input.h>
#include <Camera.h>


#include "SampleScene2/SampleScene2.h"
#include <Polygon/3D/Particle3DManager/Particle3DManager.h>

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

	uint32_t howToTexture = TextureManager::GetInstance()->LoadTexture("Resources/Sample/HowTo/HowToCamera.png");
	sprite_.reset(Sprite::Create(howToTexture, {0.0f,0.0f}));



	model1_.reset(Model::Create(model1));
	worldTransform1_.Initialize();

	model2_.reset(Model::Create(model2));
	worldTransform2_.Initialize();
	worldTransform2_.translate_ = { 0.0f,4.0f,0.0f };



	uint32_t particleTextureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");
	uint32_t particleTextureHandle2_ = TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");


	cameraTranslate_ = { 0.0f,10.0f,-30.0f };
	camera_.Initialize();
	camera_.rotate_ = { 0.2f,0.0f,0.0f };
	camera_.translate_ = cameraTranslate_;


	Particle3DManager::GetInstance()->GetInstance()->Initialize();
	
	particleTextureHandle_ = TextureManager::GetInstance()->LoadTexture("Resources/CG3/circle.png");
	uint32_t bonusTextureHandle = TextureManager::GetInstance()->LoadTexture("Resources/Sample/Ellysia/Ellysia.png");



	//同じ場所
	Particle3DManager::GetInstance()->Emit("sample1", particleTextureHandle_, { 0.0f,5.0f,0.0f }, 20);
	Particle3DManager::GetInstance()->Emit("sample2", particleTextureHandle2_, { 2.0f,1.0f,0.0f }, 5);

	Particle3DManager::GetInstance()->Emit("sample3", bonusTextureHandle, { -5.0f,6.0f,0.0f }, 3);

}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {

	

	const float SPEED = 1.0f;
	const float MOVE_AMOUNT=1.0f;


	const float CAMERA_VELOCITY = 0.05f;
	if ((Input::GetInstance()->IsPushKey(DIK_RIGHT) == true)|| (Input::GetInstance()->IsPushKey(DIK_D) == true)) {
		camera_.translate_.x += MOVE_AMOUNT * CAMERA_VELOCITY;
	}
	if ((Input::GetInstance()->IsPushKey(DIK_LEFT) == true)|| (Input::GetInstance()->IsPushKey(DIK_A) == true)) {
		camera_.translate_.x -= MOVE_AMOUNT * CAMERA_VELOCITY;
	}
	if ((Input::GetInstance()->IsPushKey(DIK_UP) == true)|| (Input::GetInstance()->IsPushKey(DIK_W) == true)) {
		camera_.translate_.y += MOVE_AMOUNT * CAMERA_VELOCITY;
	}
	if ((Input::GetInstance()->IsPushKey(DIK_DOWN) == true)|| (Input::GetInstance()->IsPushKey(DIK_S) == true)) {
		camera_.translate_.y -= MOVE_AMOUNT * CAMERA_VELOCITY;
	}


	
	//ウィンドウサイズの設定は↓でやるよ
#ifdef _DEBUG

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Rotate", &camera_.rotate_.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("Position", &camera_.translate_.x, -40.0f, 40.0f);
	ImGui::End();


#endif

	worldTransform1_.Update();
	worldTransform2_.Update();

	camera_.Update();


	if (Input::GetInstance()->IsTriggerKey(DIK_1) == true) {
		Particle3DManager::GetInstance()->DeleteElement("sample");
		Particle3DManager::GetInstance()->DeleteElement("sample2");
		Particle3DManager::GetInstance()->DeleteElement("sample3");
	}


	if (Input::GetInstance()->IsTriggerKey(DIK_SPACE) == true) {
		
		isEnd_ = true;
	}

	if (isEnd_ == true) {
		gameManager->ChangeScene(new SampleScene2());
	}



}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw() {
	
	model1_->Draw(worldTransform1_, camera_);

	model2_->Draw(worldTransform2_, camera_);
	

	Particle3DManager::GetInstance()->Draw(camera_);


	sprite_->Draw();
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
}

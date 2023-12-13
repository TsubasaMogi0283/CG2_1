#include "SampleScene.h"
#include "AllGameScene/GameManager/GameManager.h"
#include "TextureManager/TextureManager.h"
#include <Blend/BlendMode.h>
#include <Math/Quaternion/Quaternion.h>

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {

	//BlendModeを使う時は各Initializeの前で使ってね
	model_ = new Model();
	model_->SetBlendMode(BlendModeNormal);
	model_->CreateObject("Resources/Sample/fence","fence.obj");
	modelTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	spriteTransform_ = { {0.5f,0.5f,0.5f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sprite = new Sprite();
	uint32_t textureHandle = TextureManager::LoadTexture("Resources/uvChecker.png");

	sprite->SetBlendMode(BlendModeAdd);
	sprite->LoadTextureHandle(textureHandle);
	sprite->SetAllPosition({ { 0.0f,0.0f }, { 0.0f,512.0f }, { 512.0f,0.0f, }, { 512.0f,512.0f} });

	
	cameraPosition_ = Camera::GetInstance()->GetTranslate();

}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	

	Camera::GetInstance()->SetTranslate(cameraPosition_);
	
	sprite->SetColor(spriteColor_);
	model_->SetColor(modelColor_);

	Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Matrix4x4 rotatematrix = MakeRotateMatrix(rotation);


	ImGui::Begin("Rotation");
	ImGui::InputFloat("w", &rotation.w);
	ImGui::InputFloat("x", &rotation.x);
	ImGui::InputFloat("y", &rotation.y);
	ImGui::InputFloat("z", &rotation.z);
	ImGui::End();

}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete model_;
	delete sprite;
}

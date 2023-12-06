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

	
	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	
	Quaternion identity=IdentityQuaternion();
	Quaternion conj=Conjugate(q1);
	Quaternion inv=Inverse(q1);
	Quaternion normal=Normalize(q1);
	Quaternion mul1=Multiply(q1,q2);
	Quaternion mul2=Multiply(q2,q1);;
	float norm=Norm(q1);


	ImGui::Begin("Identity");
	ImGui::InputFloat("x", &identity.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("y", &identity.y, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("z", &identity.z, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("w", &identity.w, 0.01f, 1.0f, "%.2f");
	ImGui::End();

	ImGui::Begin("Conjugate");
	ImGui::InputFloat("x", &conj.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("y", &conj.y, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("z", &conj.z, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("w", &conj.w, 0.01f, 1.0f, "%.2f");
	ImGui::End();

	ImGui::Begin("Inverse");
	ImGui::InputFloat("x", &inv.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("y", &inv.y, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("z", &inv.z, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("w", &inv.w, 0.01f, 1.0f, "%.2f");
	ImGui::End();

	ImGui::Begin("Normalize");
	ImGui::InputFloat("x", &normal.x,0.01f,1.0f,"%.2f");
	ImGui::InputFloat("y", &normal.y,0.01f,1.0f,"%.2f");
	ImGui::InputFloat("z", &normal.z,0.01f,1.0f,"%.2f");
	ImGui::InputFloat("w", &normal.w,0.01f,1.0f,"%.2f");
	ImGui::End();

	ImGui::Begin("Mul1");
	ImGui::InputFloat("x", &mul1.x,0.01f,1.0f,"%.2f");
	ImGui::InputFloat("y", &mul1.y,0.01f,1.0f,"%.2f");
	ImGui::InputFloat("z", &mul1.z,0.01f,1.0f,"%.2f");
	ImGui::InputFloat("w", &mul1.w,0.01f,1.0f,"%.2f");
	ImGui::End();

	ImGui::Begin("Mul2");
	ImGui::InputFloat("x", &mul2.x,0.01f,1.0f,"%.2f");
	ImGui::InputFloat("y", &mul2.y,0.01f,1.0f,"%.2f");
	ImGui::InputFloat("z", &mul2.z,0.01f,1.0f,"%.2f");
	ImGui::InputFloat("w", &mul2.w,0.01f,1.0f,"%.2f");
	ImGui::End();

	ImGui::Begin("Norm");
	ImGui::InputFloat("Norm", &norm, 0.01f, 1.0f, "%.2f");
	ImGui::End();

}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	//model_->Draw(modelTransform_);
	//sprite->DrawRect(spriteTransform_);
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete model_;
	delete sprite;
}

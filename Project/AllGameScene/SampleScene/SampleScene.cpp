#include "SampleScene.h"
#include "AllGameScene/GameManager/GameManager.h"
#include "TextureManager/TextureManager.h"
#include <Blend/BlendMode.h>

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

	

	Vector3 from0 = Normalize(Vector3{ 1.0f,0.7f,0.5f });
	Vector3 to0 = { -from0.x,-from0.y,-from0.z };
	Vector3 from1 = Normalize(Vector3{ -0.6f,0.9f,0.2f });
	Vector3 to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });


#pragma region RotateMatrix0

	/*Matrix4x4 rotateMatrix0 = DirectionToDirection(
		Normalize(Vector3{1.0f,0.0f,0.0f}), 
		Normalize(Vector3{-1.0f,0.0f,0.0f}));*/




	/*ImGui::Begin("Rotatematrix0");
	ImGui::InputFloat("m[0][0]", &rotateMatrix0.m[0][0]);
	ImGui::InputFloat("m[0][1]", &rotateMatrix0.m[0][1]);
	ImGui::InputFloat("m[0][2]", &rotateMatrix0.m[0][2]);
	ImGui::InputFloat("m[0][3]", &rotateMatrix0.m[0][3]);

	ImGui::InputFloat("m[1][0]", &rotateMatrix0.m[1][0]);
	ImGui::InputFloat("m[1][1]", &rotateMatrix0.m[1][1]);
	ImGui::InputFloat("m[1][2]", &rotateMatrix0.m[1][2]);
	ImGui::InputFloat("m[1][3]", &rotateMatrix0.m[1][3]);

	ImGui::InputFloat("m[2][0]", &rotateMatrix0.m[2][0]);
	ImGui::InputFloat("m[2][1]", &rotateMatrix0.m[2][1]);
	ImGui::InputFloat("m[2][2]", &rotateMatrix0.m[2][2]);
	ImGui::InputFloat("m[2][3]", &rotateMatrix0.m[2][3]);

	ImGui::InputFloat("m[3][0]", &rotateMatrix0.m[3][0]);
	ImGui::InputFloat("m[3][1]", &rotateMatrix0.m[3][1]);
	ImGui::InputFloat("m[3][2]", &rotateMatrix0.m[3][2]);
	ImGui::InputFloat("m[3][3]", &rotateMatrix0.m[3][3]);

	ImGui::End();*/

#pragma endregion 

	Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);

	ImGui::Begin("RotateMatrix1");
	ImGui::InputFloat("m[0][0]", &rotateMatrix1.m[0][0]);
	ImGui::InputFloat("m[0][1]", &rotateMatrix1.m[0][1]);
	ImGui::InputFloat("m[0][2]", &rotateMatrix1.m[0][2]);
	ImGui::InputFloat("m[0][3]", &rotateMatrix1.m[0][3]);

	ImGui::InputFloat("m[1][0]", &rotateMatrix1.m[1][0]);
	ImGui::InputFloat("m[1][1]", &rotateMatrix1.m[1][1]);
	ImGui::InputFloat("m[1][2]", &rotateMatrix1.m[1][2]);
	ImGui::InputFloat("m[1][3]", &rotateMatrix1.m[1][3]);

	ImGui::InputFloat("m[2][0]", &rotateMatrix1.m[2][0]);
	ImGui::InputFloat("m[2][1]", &rotateMatrix1.m[2][1]);
	ImGui::InputFloat("m[2][2]", &rotateMatrix1.m[2][2]);
	ImGui::InputFloat("m[2][3]", &rotateMatrix1.m[2][3]);

	ImGui::InputFloat("m[3][0]", &rotateMatrix1.m[3][0]);
	ImGui::InputFloat("m[3][1]", &rotateMatrix1.m[3][1]);
	ImGui::InputFloat("m[3][2]", &rotateMatrix1.m[3][2]);
	ImGui::InputFloat("m[3][3]", &rotateMatrix1.m[3][3]);

	ImGui::End();

	Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

	ImGui::Begin("RotateMatrix2");
	ImGui::InputFloat("m[0][0]", &rotateMatrix2.m[0][0]);
	ImGui::InputFloat("m[0][1]", &rotateMatrix2.m[0][1]);
	ImGui::InputFloat("m[0][2]", &rotateMatrix2.m[0][2]);
	ImGui::InputFloat("m[0][3]", &rotateMatrix2.m[0][3]);

	ImGui::InputFloat("m[1][0]", &rotateMatrix2.m[1][0]);
	ImGui::InputFloat("m[1][1]", &rotateMatrix2.m[1][1]);
	ImGui::InputFloat("m[1][2]", &rotateMatrix2.m[1][2]);
	ImGui::InputFloat("m[1][3]", &rotateMatrix2.m[1][3]);

	ImGui::InputFloat("m[2][0]", &rotateMatrix2.m[2][0]);
	ImGui::InputFloat("m[2][1]", &rotateMatrix2.m[2][1]);
	ImGui::InputFloat("m[2][2]", &rotateMatrix2.m[2][2]);
	ImGui::InputFloat("m[2][3]", &rotateMatrix2.m[2][3]);

	ImGui::InputFloat("m[3][0]", &rotateMatrix2.m[3][0]);
	ImGui::InputFloat("m[3][1]", &rotateMatrix2.m[3][1]);
	ImGui::InputFloat("m[3][2]", &rotateMatrix2.m[3][2]);
	ImGui::InputFloat("m[3][3]", &rotateMatrix2.m[3][3]);

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

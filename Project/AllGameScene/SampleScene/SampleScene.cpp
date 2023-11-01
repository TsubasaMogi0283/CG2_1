#include "SampleScene.h"
#include "AllGameScene/GameManager/GameManager.h"
#include "TextureManager/TextureManager.h"

/// <summary>
	/// コンストラクタ
	/// </summary>
SampleScene::SampleScene() {

}



/// <summary>
/// 初期化
/// </summary>
void SampleScene::Initialize(GameManager* gameManager) {
	model_ = new Model();
	model_->CreateObject("Resources/05_02","plane.obj");
	modelTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	if (input_->GetInstance()->IsPushKey(DIK_SPACE) == true) {
		if (isActivateCamera_ == false) {
			isActivateCamera_ = true;
		}
		
	}
	else {
		isActivateCamera_ = false;
	}

	if (isActivateCamera_ == true) {
		debugCamera_->Update();
		Camera::GetInstance()->SetViewMatrix(debugCamera_->GetViewProjection().matView_);
		Camera::GetInstance()->SetProjectionMatrix(debugCamera_->GetViewProjection().matProjection_);

		ImGui::Begin("debugcamera");

		ImGui::InputFloat("cameraPosition", &debugCamera_->GetViewProjection().matView_.m[0][0]);
		ImGui::InputFloat("cameraPosition", &debugCamera_->GetViewProjection().matView_.m[0][1]);
		ImGui::InputFloat("cameraPosition", &debugCamera_->GetViewProjection().matView_.m[0][2]);
		ImGui::InputFloat("cameraPosition", &debugCamera_->GetViewProjection().matView_.m[0][3]);



		ImGui::End();

		
	}
	
	
}

/// <summary>
/// 描画
/// </summary>
void SampleScene::Draw(GameManager* gameManager) {
	model_->Draw(modelTransform_);
}

/// <summary>
/// デストラクタ
/// </summary>
SampleScene::~SampleScene() {
	delete model_;
}

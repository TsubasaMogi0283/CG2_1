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

	
}

/// <summary>
/// 更新
/// </summary>
void SampleScene::Update(GameManager* gameManager) {
	Quaternion rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f,0.71f,0.0f }, 0.3f);
	Quaternion rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f,0.0f,0.71f }, 3.141592f);

	Quaternion interpolate0 = QuaternionSlerp(rotation0, rotation1, 0.0f);
	Quaternion interpolate1 = QuaternionSlerp(rotation0, rotation1, 0.3f);
	Quaternion interpolate2 = QuaternionSlerp(rotation0, rotation1, 0.5f);
	Quaternion interpolate3 = QuaternionSlerp(rotation0, rotation1, 0.7f);
	Quaternion interpolate4 = QuaternionSlerp(rotation0, rotation1, 1.0f);

	ImGui::Begin("interpolate");
	ImGui::InputFloat4("interpolate0", &interpolate0.x);
	ImGui::InputFloat4("interpolate1", &interpolate1.x);
	ImGui::InputFloat4("interpolate2", &interpolate2.x);
	ImGui::InputFloat4("interpolate3", &interpolate3.x);
	ImGui::InputFloat4("interpolate4", &interpolate4.x);

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
}

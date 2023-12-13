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
	


	Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotationByMatrix = TransformCalculation(pointY, rotateMatrix);

	ImGui::Begin("Rotation");
	ImGui::InputFloat("x", &rotation.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("y", &rotation.y, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("z", &rotation.z, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("w", &rotation.w, 0.01f, 1.0f, "%.2f");
	ImGui::End();


	ImGui::Begin("RotateMatrix");
	ImGui::InputFloat("m.[0][0]", &rotateMatrix.m[0][0],0.0f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[0][1]", &rotateMatrix.m[0][1],0.0f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[0][2]", &rotateMatrix.m[0][2],0.0f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[0][3]", &rotateMatrix.m[0][3],0.0f, 1.0f, "%.3f");

	ImGui::InputFloat("m.[1][0]", &rotateMatrix.m[1][0],0.0f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[1][1]", &rotateMatrix.m[1][1],0.0f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[1][2]", &rotateMatrix.m[1][2],0.0f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[1][3]", &rotateMatrix.m[1][3],0.0f, 1.0f, "%.3f");

	ImGui::InputFloat("m.[2][0]", &rotateMatrix.m[2][0], 0.01f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[2][1]", &rotateMatrix.m[2][1], 0.01f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[2][2]", &rotateMatrix.m[2][2], 0.01f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[2][3]", &rotateMatrix.m[2][3], 0.01f, 1.0f, "%.3f");

	ImGui::InputFloat("m.[3][0]", &rotateMatrix.m[3][0], 0.01f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[3][1]", &rotateMatrix.m[3][1], 0.01f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[3][2]", &rotateMatrix.m[3][2], 0.01f, 1.0f, "%.3f");
	ImGui::InputFloat("m.[3][3]", &rotateMatrix.m[3][3], 0.01f, 1.0f, "%.3f");


	ImGui::End();



	ImGui::Begin("RotateByQuaternion");
	ImGui::InputFloat("x", &rotateByQuaternion.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("y", &rotateByQuaternion.y, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("z", &rotateByQuaternion.z, 0.01f, 1.0f, "%.2f");
	ImGui::End();

	ImGui::Begin("RotateByMatrix");
	ImGui::InputFloat("x", &rotationByMatrix.x, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("y", &rotationByMatrix.y, 0.01f, 1.0f, "%.2f");
	ImGui::InputFloat("z", &rotationByMatrix.z, 0.01f, 1.0f, "%.2f");
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

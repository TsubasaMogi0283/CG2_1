#include "Camera.h"
#include <Matrix4x4Calculation.h>
#include "WindowsSetup.h"


////コンストラクタ
//Camera::Camera() {
//	//コンストラクタの所で値を入れる
//	//わざわざInitialize関数を作るのは面倒だから
//	//デフォルト
//	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-9.8f} };
//}
//
////インスタンス
//Camera* Camera::GetInstance() {
//	//関数内static変数として宣言する
//	static Camera instance;
//
//	return &instance;
//	
//}
//
//Matrix4x4 Camera::GetAffineMatrix() {
//	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
//	return cameraMatrix_;
//}
//
//Matrix4x4 Camera::GetViewMatrix() {
//	//カメラ行列
//	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
//	viewMatrix_ = Inverse(cameraMatrix_);
//	return viewMatrix_;
//}
//
//Matrix4x4 Camera::GetProjectionMatrix_() {
//	//遠視投影行列
//	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(WindowsSetup::GetInstance()->GetClientWidth()) / float(WindowsSetup::GetInstance()->GetClientHeight()), 0.1f, 100.0f);
//	
//	return projectionMatrix_;
//}
//
//
//
//
////デストラクタ
//Camera::~Camera() {
//
//}









#include "Camera.h"
#include <Matrix4x4Calculation.h>
#include "WindowsSetup.h"



//初期化
void Camera::Initialize() {
	//Resource作成
	bufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraMatrixData)).Get();

	aspectRatio_ = float(WindowsSetup::GetInstance()->GetClientWidth()) / float(WindowsSetup::GetInstance()->GetClientHeight());

	//初期
	scale_ = { 1.0f, 1.0f,1.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = { 0.0f, 0.0f, 0.0f };

	//メインはUpdateの方
	//アフィン行列を計算
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);
	//カメラと言えば逆行列
	viewMatrix_ = Inverse(worldMatrix_);
	//射影を計算
	projectionMatrix_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
	//正射影行列(正規化)を計算
	orthographicMatrix_ = MakeOrthographicMatrix(0, 0, float(WindowsSetup::GetInstance()->GetClientWidth()), float(WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);

}

//行列を計算・転送する
void Camera::Update() {

	//アフィン行列を計算
	worldMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);

	//カメラと言えば逆行列
	viewMatrix_ = Inverse(worldMatrix_);
	//射影を計算
	projectionMatrix_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
	//正射影行列(正規化)を計算
	orthographicMatrix_ = MakeOrthographicMatrix(0, 0, float(WindowsSetup::GetInstance()->GetClientWidth()), float(WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);

	//転送
	Transfer();
}

void Camera::Transfer() {
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraMatrixData_));
	cameraMatrixData_->viewMatrix_ = viewMatrix_;
	cameraMatrixData_->projectionMatrix_ = projectionMatrix_;
	cameraMatrixData_->orthographicMatrix_ = orthographicMatrix_;
	bufferResource_->Unmap(0, nullptr);
}
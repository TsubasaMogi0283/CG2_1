#include "Camera.h"
#include <Matrix4x4Calculation.h>
#include "WindowsSetup.h"




//コンストラクタ
Camera::Camera() {
	//コンストラクタの所で値を入れる
	//デフォルト
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-9.8f} };
}

//インスタンス
Camera* Camera::GetInstance() {
	//関数内static変数として宣言する
	static Camera instance;

	return &instance;
	
}

//シーンを変えて初期化したいときはこれを使う
void Camera::Initialize() {
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-40.0f} };
}

Matrix4x4 Camera::GetAffineMatrix(){
	Matrix4x4 result = {};
	result = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	return result;
}


Matrix4x4 Camera::GetViewMatrix() {
	//カメラ行列
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	return viewMatrix_;
}

Matrix4x4 Camera::GetProjectionMatrix_() {
	//遠視投影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(WindowsSetup::GetInstance()->GetClientWidth()) / float(WindowsSetup::GetInstance()->GetClientHeight()), 0.1f, 100.0f);
	
	return projectionMatrix_;
}




//デストラクタ
Camera::~Camera() {

}


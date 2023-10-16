#include "Camera.h"
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

Camera* Camera::instance_=nullptr;

//コンストラクタ
Camera::Camera() {
	//コンストラクタの所で値を入れる
	//わざわざInitialize関数を作るのは面倒だから
	//デフォルト
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-9.8f} };
}

//インスタンス
Camera* Camera::GetInstance() {
	//これだと無限に生成される
	if (instance_ == nullptr) {
		instance_ = new Camera();

	}
	
	return instance_;
}


Matrix4x4 Camera::GetViewMatrix() {
	//カメラ行列
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	return viewMatrix_;
}

Matrix4x4 Camera::GetProjectionMatrix_() {
	//遠視投影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(DirectXSetup::GetInstance()->GetClientWidth()) / float(DirectXSetup::GetInstance()->GetClientHeight()), 0.1f, 100.0f);
	
	return projectionMatrix_;
}




//デリート代わりの関数
void Camera::DeleteInstance() {
	delete instance_;
}

//デストラクタ
Camera::~Camera() {

}


#include "Camera.h"

//コンストラクタ
Camera::Camera() {

}

//インスタンス
Camera* Camera::GetInstance() {
	//これだと無限に生成される
	if (instance_ == nullptr) {
		instance_ = new Camera();

	}
	
	return instance_;
}

void Camera::Initialize() {
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-9.8f} };
}

void Camera::Update() {
	//カメラ行列
	cameraMatrix_ = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	
	//遠視投影行列
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(DirectXSetup::GetInstance()->GetClientWidth()) / float(DirectXSetup::GetInstance()->GetClientHeight()), 0.1f, 100.0f);
	
}


//デリート代わりの関数
void Camera::DeleteInstance() {
	delete instance_;
}

//デストラクタ
Camera::~Camera() {

}


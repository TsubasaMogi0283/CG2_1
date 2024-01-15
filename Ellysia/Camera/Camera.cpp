#include "Camera.h"
#include <Matrix4x4Calculation.h>
#include "WindowsSetup.h"



//初期化
void Camera::Initialize() {
	//Resource作成
	bufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraBufferData)).Get();

	aspectRatio_ = float(WindowsSetup::GetInstance()->GetClientWidth()) / float(WindowsSetup::GetInstance()->GetClientHeight());

	//初期
	scale_ = { 1.0f, 1.0f,1.0f };
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = { 0.0f, 0.0f, 0.0f };
}

//行列を計算・転送する
void Camera::Update() {
	
	//アフィン行列を計算
	affineMatrix_ = MakeAffineMatrix(scale_, rotate_, translate_);
	
	//カメラと言えば逆行列
	viewMatrix_ = Inverse(affineMatrix_);
	//射影を計算
	matProjection_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
	//正射影行列(正規化)を計算
	orthographicMatrix_ = MakeOrthographicMatrix(0, 0, float(WindowsSetup::GetInstance()->GetClientWidth()), float(WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);

	//転送
	//Camera::Transfer();
}


//転送
void Camera::Transfer() {
	//Resourceに書き込む
	bufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&bufferData_));


	//WVP行列を作成
	//Matrix4x4 worldViewProjectionMatrix = Multiply(matWorld_, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));

	//データ書き込み
	bufferData_->viewMatrix = viewMatrix_;
	bufferData_->projectionMatrix = matProjection_;
	bufferData_->orthographicMatrix = orthographicMatrix_;
	bufferData_->position = translate_;

	//KAMATA ENGINEにorthographicが無かったので追加しておく

	bufferResource_->Unmap(0, nullptr);
}


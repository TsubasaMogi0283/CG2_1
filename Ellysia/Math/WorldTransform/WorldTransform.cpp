#include "WorldTransform.h"
#include "Matrix4x4Calculation.h"

#include "DirectXSetup.h"
#include <Camera.h>

void WorldTransform::Initialize(){
	//Resource作成
	constBufferResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ConstBuffDataWorldTransform)).Get();

	scale_ = {1.0f, 1.0f,1.0f};
	rotate_ = { 0.0f, 0.0f, 0.0f };
	translate_ = {0.0f, 0.0f, 0.0f};
}

void WorldTransform::Update(){
	//SRT合成
	matWorld_ = MakeAffineMatrix(scale_, rotate_, translate_);

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	WorldTransform::Transfer();
}


void WorldTransform::Transfer(){
	//Resourceに書き込む
	//今までTransformationに書いていたものをこっちに引っ越す
	constBufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&tranceformationData_));

	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrix = Multiply(matWorld_, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));


	tranceformationData_->wvp = worldViewProjectionMatrix;
	//tranceformationData_->World =MakeIdentity4x4();
	tranceformationData_->world = MakeIdentity4x4();
	constBufferResource_->Unmap(0, nullptr);
}

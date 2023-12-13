#include "WorldTransform.h"
#include "Matrix4x4Calculation.h"

#include "DirectXSetup.h"

void WorldTransform::Initialize(){
	//Resource作成
	constBuffer_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ConstBuffDataWorldTransform)).Get();

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
	ConstBuffDataWorldTransform* worldTransformData = nullptr;
	constBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&worldTransformData));
	worldTransformData->world = matWorld_;
	constBuffer_->Unmap(0, nullptr);
}

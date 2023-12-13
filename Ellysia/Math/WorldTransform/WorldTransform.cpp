#include "WorldTransform.h"
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

void WorldTransform::Initialize(){
	scale_ = {1.0f, 1, 1};
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
}

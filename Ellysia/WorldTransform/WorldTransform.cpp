#include "WorldTransform.h"
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

/// <summary>
/// 行列を計算・転送する
/// </summary>
void WorldTransform::UpdateMatrix() {
	//ALとほぼ同じだね
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}
}
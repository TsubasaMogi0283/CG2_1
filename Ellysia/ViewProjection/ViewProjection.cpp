#include "ViewProjection.h"
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

void ViewProjection::UpdateMatrix() {


	//Scaleは必要ないでしょう
	//ビューだからInverseを使うよ
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate_);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate_.x);
	Matrix4x4 rotateYMatrix = MakeRotateXMatrix(rotate_.y);
	Matrix4x4 rotateZMatrix = MakeRotateXMatrix(rotate_.z);


	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 inverseTranslate = Inverse(translateMatrix);

	//ビュー行列の計算
	matView_ = Multiply(inverseTranslate, Inverse(rotateMatrix));

	//プロジェクション行列の計算
	matProjection_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
}


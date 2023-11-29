#include "Matrix4x4Calculation.h"
#include "Math/Vector/Calculation/VectorCalculation.h"
#include <Math/Matrix/Matrix/Matrix3x3.h>
#include <Math/Vector/Vector4.h>



//コタンジェント
float Cot(float theta) {
	return (1.0f / tan(theta));
}

Matrix3x3 MultiplyValueMatrix(float v1, Matrix3x3 m1) {
	Matrix3x3 result = {};
	result.m[0][0] = v1 * m1.m[0][0];
	result.m[0][1] = v1 * m1.m[0][1];
	result.m[0][2] = v1 * m1.m[0][2];


	result.m[1][0] = v1 * m1.m[0][1];
	result.m[1][1] = v1 * m1.m[1][1];
	result.m[1][2] = v1 * m1.m[1][2];
	
	result.m[2][0] = v1 * m1.m[2][0];
	result.m[2][1] = v1 * m1.m[2][2];
	result.m[2][2] = v1 * m1.m[2][2];

	return result;
}

Matrix3x3 MultiplyVectorMatrix(Vector3 v1, Matrix3x3 m1) {
	Matrix3x3 result = {};
	result.m[0][0] = v1.x * m1.m[0][0];
	result.m[0][1] = v1.y * m1.m[1][0];
	result.m[0][2] = v1.z * m1.m[2][0];


	result.m[1][0] = v1.x * m1.m[0][1];
	result.m[1][1] = v1.y * m1.m[1][1];
	result.m[1][2] = v1.z * m1.m[2][1];
	
	result.m[2][0] = v1.x * m1.m[0][2];
	result.m[2][1] = v1.y * m1.m[1][2];
	result.m[2][2] = v1.z* m1.m[2][2];

	return result;
}

Matrix3x3 MakeIdentity3x3() {
	Matrix3x3 result = {
		result.m[0][0] = 1.0f,
		result.m[0][1] = 0.0f,
		result.m[0][2] = 0.0f,

		result.m[1][0] = 0.0f,
		result.m[1][1] = 1.0f,
		result.m[1][2] = 0.0f,

		result.m[2][0] = 0.0f,
		result.m[2][1] = 0.0f,
		result.m[2][2] = 1.0f,

	};

	return result;
}

//単位行列を作成する
//斜めの1が並ぶやつ
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result = {
		result.m[0][0] = 1.0f,
		result.m[0][1] = 0.0f,
		result.m[0][2] = 0.0f,
		result.m[0][3] = 0.0f,

		result.m[1][0] = 0.0f,
		result.m[1][1] = 1.0f,
		result.m[1][2] = 0.0f,
		result.m[1][3] = 0.0f,

		result.m[2][0] = 0.0f,
		result.m[2][1] = 0.0f,
		result.m[2][2] = 1.0f,
		result.m[2][3] = 0.0f,

		result.m[3][0] = 0.0f,
		result.m[3][1] = 0.0f,
		result.m[3][2] = 0.0f,
		result.m[3][3] = 1.0f,
	};

	return result;
}




//乗算
Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2) {
	Matrix4x4 result = {};
	result.m[0][0] = (m1.m[0][0] * m2.m[0][0]) + (m1.m[0][1] * m2.m[1][0]) + (m1.m[0][2] * m2.m[2][0]) + (m1.m[0][3] * m2.m[3][0]);
	result.m[0][1] = (m1.m[0][0] * m2.m[0][1]) + (m1.m[0][1] * m2.m[1][1]) + (m1.m[0][2] * m2.m[2][1]) + (m1.m[0][3] * m2.m[3][1]);
	result.m[0][2] = (m1.m[0][0] * m2.m[0][2]) + (m1.m[0][1] * m2.m[1][2]) + (m1.m[0][2] * m2.m[2][2]) + (m1.m[0][3] * m2.m[3][2]);
	result.m[0][3] = (m1.m[0][0] * m2.m[0][3]) + (m1.m[0][1] * m2.m[1][3]) + (m1.m[0][2] * m2.m[2][3]) + (m1.m[0][3] * m2.m[3][3]);

	result.m[1][0] = (m1.m[1][0] * m2.m[0][0]) + (m1.m[1][1] * m2.m[1][0]) + (m1.m[1][2] * m2.m[2][0]) + (m1.m[1][3] * m2.m[3][0]);
	result.m[1][1] = (m1.m[1][0] * m2.m[0][1]) + (m1.m[1][1] * m2.m[1][1]) + (m1.m[1][2] * m2.m[2][1]) + (m1.m[1][3] * m2.m[3][1]);
	result.m[1][2] = (m1.m[1][0] * m2.m[0][2]) + (m1.m[1][1] * m2.m[1][2]) + (m1.m[1][2] * m2.m[2][2]) + (m1.m[1][3] * m2.m[3][2]);
	result.m[1][3] = (m1.m[1][0] * m2.m[0][3]) + (m1.m[1][1] * m2.m[1][3]) + (m1.m[1][2] * m2.m[2][3]) + (m1.m[1][3] * m2.m[3][3]);

	result.m[2][0] = (m1.m[2][0] * m2.m[0][0]) + (m1.m[2][1] * m2.m[1][0]) + (m1.m[2][2] * m2.m[2][0]) + (m1.m[2][3] * m2.m[3][0]);
	result.m[2][1] = (m1.m[2][0] * m2.m[0][1]) + (m1.m[2][1] * m2.m[1][1]) + (m1.m[2][2] * m2.m[2][1]) + (m1.m[2][3] * m2.m[3][1]);
	result.m[2][2] = (m1.m[2][0] * m2.m[0][2]) + (m1.m[2][1] * m2.m[1][2]) + (m1.m[2][2] * m2.m[2][2]) + (m1.m[2][3] * m2.m[3][2]);
	result.m[2][3] = (m1.m[2][0] * m2.m[0][3]) + (m1.m[2][1] * m2.m[1][3]) + (m1.m[2][2] * m2.m[2][3]) + (m1.m[2][3] * m2.m[3][3]);

	result.m[3][0] = (m1.m[3][0] * m2.m[0][0]) + (m1.m[3][1] * m2.m[1][0]) + (m1.m[3][2] * m2.m[2][0]) + (m1.m[3][3] * m2.m[3][0]);
	result.m[3][1] = (m1.m[3][0] * m2.m[0][1]) + (m1.m[3][1] * m2.m[1][1]) + (m1.m[3][2] * m2.m[2][1]) + (m1.m[3][3] * m2.m[3][1]);
	result.m[3][2] = (m1.m[3][0] * m2.m[0][2]) + (m1.m[3][1] * m2.m[1][2]) + (m1.m[3][2] * m2.m[2][2]) + (m1.m[3][3] * m2.m[3][2]);
	result.m[3][3] = (m1.m[3][0] * m2.m[0][3]) + (m1.m[3][1] * m2.m[1][3]) + (m1.m[3][2] * m2.m[2][3]) + (m1.m[3][3] * m2.m[3][3]);


	return result;


}



//Scale
//拡縮
Matrix4x4 MakeScaleMatrix(const Vector3 scale) {
	Matrix4x4 result = {};
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;


	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;


	return result;
}


//Rotate
#pragma region XYZの個別の回転

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = {};

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = -(std::sin(radian));
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0.0f;
	result.m[0][2] = -(std::sin(radian));
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0.0f;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = -(std::sin(radian));
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

#pragma endregion 

//AllRotate
//回転
Matrix4x4 MakeRotateXYZMatrix(float radianX, float radianY, float radianZ) {
	Matrix4x4 result;

	Matrix4x4 rotateMatrixX = MakeRotateXMatrix(radianX);
	Matrix4x4 rotateMatrixY = MakeRotateYMatrix(radianY);
	Matrix4x4 rotateMatrixZ = MakeRotateZMatrix(radianZ);



	result = Multiply(rotateMatrixX, Multiply(rotateMatrixY, rotateMatrixZ));

	return result;

}



//Translate
//移動
Matrix4x4 MakeTranslateMatrix(Vector3 translate) {
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;


	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;


	return result;
}



//AffineMatrix
//SRTの融合
//アフィン行列
Matrix4x4 MakeAffineMatrix(const Vector3 scale, const Vector3 rotate, const Vector3 translate) {
	Matrix4x4 result;

	//S
	Matrix4x4 scaleMatrix;
	scaleMatrix = MakeScaleMatrix(scale);

	//R
	Matrix4x4 rotateMatrix;
	rotateMatrix = MakeRotateXYZMatrix(rotate.x, rotate.y, rotate.z);

	Matrix4x4 translateMatrix;
	translateMatrix = MakeTranslateMatrix(translate);


	result = Multiply(scaleMatrix, Multiply(rotateMatrix, translateMatrix));

	return result;
}


//逆行列
Matrix4x4 Inverse(const Matrix4x4 m) {
	float MatrixFormula;
	MatrixFormula =
		+(m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3])
		+ (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1])
		+ (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2])

		- (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1])
		- (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3])
		- (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2])

		- (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3])
		- (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1])
		- (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2])

		+ (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1])
		+ (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3])
		+ (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2])

		+ (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3])
		+ (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1])
		+ (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2])


		- (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1])
		- (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3])
		- (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2])

		- (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0])
		- (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0])
		- (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0])

		+ (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0])
		+ (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0])
		+ (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	Matrix4x4 result = {};



	result.m[0][0] = (1 / MatrixFormula) * (
		+(m.m[1][1] * m.m[2][2] * m.m[3][3])
		+ (m.m[1][2] * m.m[2][3] * m.m[3][1])
		+ (m.m[1][3] * m.m[2][1] * m.m[3][2])
		- (m.m[1][3] * m.m[2][2] * m.m[3][1])
		- (m.m[1][2] * m.m[2][1] * m.m[3][3])
		- (m.m[1][1] * m.m[2][3] * m.m[3][2]));

	result.m[0][1] = (1 / MatrixFormula) * (
		-(m.m[0][2] * m.m[2][3] * m.m[3][3])
		- (m.m[0][2] * m.m[2][3] * m.m[3][1])
		- (m.m[0][3] * m.m[2][1] * m.m[3][2])
		+ (m.m[0][3] * m.m[2][2] * m.m[0][2])
		+ (m.m[0][2] * m.m[2][1] * m.m[3][3])
		+ (m.m[0][1] * m.m[2][3] * m.m[3][2]));


	result.m[0][2] = (1 / MatrixFormula) * (
		+(m.m[0][1] * m.m[1][2] * m.m[3][3])
		+ (m.m[0][2] * m.m[1][3] * m.m[3][1])
		+ (m.m[0][3] * m.m[1][1] * m.m[3][2])
		- (m.m[0][3] * m.m[1][2] * m.m[3][1])
		- (m.m[0][2] * m.m[1][1] * m.m[3][3])
		- (m.m[0][1] * m.m[1][3] * m.m[3][2]));

	result.m[0][3] = (1 / MatrixFormula) * (
		-(m.m[0][1] * m.m[1][2] * m.m[2][3])
		- (m.m[0][2] * m.m[1][3] * m.m[2][1])
		- (m.m[0][3] * m.m[1][1] * m.m[2][2])
		+ (m.m[0][3] * m.m[1][2] * m.m[2][1])
		+ (m.m[0][2] * m.m[1][1] * m.m[2][3])
		+ (m.m[0][1] * m.m[1][3] * m.m[2][2]));



	result.m[1][0] = (1 / MatrixFormula) * (
		-(m.m[1][0] * m.m[2][2] * m.m[3][3])
		- (m.m[1][2] * m.m[2][3] * m.m[3][0])
		- (m.m[1][3] * m.m[2][0] * m.m[3][2])
		+ (m.m[1][3] * m.m[2][2] * m.m[3][0])
		+ (m.m[1][2] * m.m[2][0] * m.m[3][3])
		+ (m.m[1][0] * m.m[2][3] * m.m[3][2]));

	result.m[1][1] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[2][2] * m.m[3][3])
		+ (m.m[0][2] * m.m[2][3] * m.m[3][0])
		+ (m.m[0][3] * m.m[2][0] * m.m[3][2])
		- (m.m[0][3] * m.m[2][2] * m.m[3][0])
		- (m.m[0][2] * m.m[2][0] * m.m[3][3])
		- (m.m[0][0] * m.m[2][3] * m.m[3][2]));

	result.m[1][2] = (1 / MatrixFormula) * (
		-(m.m[0][0] * m.m[1][2] * m.m[3][3])
		- (m.m[0][2] * m.m[1][3] * m.m[3][0])
		- (m.m[0][3] * m.m[1][0] * m.m[3][2])
		+ (m.m[0][3] * m.m[1][2] * m.m[3][0])
		+ (m.m[0][2] * m.m[1][0] * m.m[3][3])
		+ (m.m[0][0] * m.m[1][3] * m.m[3][2]));

	result.m[1][3] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[1][2] * m.m[2][3])
		+ (m.m[0][2] * m.m[1][3] * m.m[2][0])
		+ (m.m[0][3] * m.m[1][0] * m.m[2][2])
		- (m.m[0][3] * m.m[1][2] * m.m[2][0])
		- (m.m[0][2] * m.m[1][0] * m.m[2][3])
		- (m.m[0][0] * m.m[1][3] * m.m[2][2]));



	result.m[2][0] = (1 / MatrixFormula) * (
		+(m.m[1][0] * m.m[2][1] * m.m[3][3])
		+ (m.m[1][1] * m.m[2][3] * m.m[3][0])
		+ (m.m[1][3] * m.m[2][0] * m.m[3][1])
		- (m.m[1][3] * m.m[2][1] * m.m[3][0])
		- (m.m[1][1] * m.m[2][0] * m.m[3][3])
		- (m.m[1][0] * m.m[2][3] * m.m[3][1]));

	result.m[2][1] = (1 / MatrixFormula) * (
		-(m.m[0][0] * m.m[2][1] * m.m[3][3])
		- (m.m[0][1] * m.m[2][3] * m.m[3][0])
		- (m.m[0][3] * m.m[2][0] * m.m[3][1])
		+ (m.m[0][3] * m.m[2][1] * m.m[3][0])
		+ (m.m[0][1] * m.m[2][0] * m.m[3][3])
		+ (m.m[0][0] * m.m[2][3] * m.m[3][1]));

	result.m[2][2] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[1][1] * m.m[3][3])
		+ (m.m[0][1] * m.m[1][3] * m.m[3][0])
		+ (m.m[0][3] * m.m[1][0] * m.m[3][1])
		- (m.m[0][3] * m.m[1][1] * m.m[3][0])
		- (m.m[0][1] * m.m[1][0] * m.m[3][3])
		- (m.m[0][0] * m.m[1][3] * m.m[3][1]));

	result.m[2][3] = (1 / MatrixFormula) * (
		-(m.m[0][0] * m.m[1][1] * m.m[2][3])
		- (m.m[0][1] * m.m[1][3] * m.m[2][0])
		- (m.m[0][3] * m.m[1][0] * m.m[2][1])
		+ (m.m[0][3] * m.m[1][1] * m.m[2][0])
		+ (m.m[0][1] * m.m[1][0] * m.m[2][3])
		+ (m.m[0][0] * m.m[1][3] * m.m[2][1]));


	result.m[3][0] = (1 / MatrixFormula) * (
		-(m.m[1][0] * m.m[2][1] * m.m[3][2])
		- (m.m[1][1] * m.m[2][2] * m.m[3][0])
		- (m.m[1][2] * m.m[2][0] * m.m[3][1])
		+ (m.m[1][2] * m.m[2][1] * m.m[3][0])
		+ (m.m[1][1] * m.m[2][0] * m.m[3][2])
		+ (m.m[1][0] * m.m[2][2] * m.m[3][1]));


	result.m[3][1] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[2][1] * m.m[3][2])
		+ (m.m[0][1] * m.m[2][2] * m.m[3][0])
		+ (m.m[0][2] * m.m[2][0] * m.m[3][1])
		- (m.m[0][2] * m.m[2][1] * m.m[3][0])
		- (m.m[0][1] * m.m[2][0] * m.m[3][2])
		- (m.m[0][0] * m.m[2][2] * m.m[3][1]));

	result.m[3][2] = (1 / MatrixFormula) * (
		-(m.m[0][0] * m.m[1][1] * m.m[3][2])
		- (m.m[0][1] * m.m[1][2] * m.m[3][0])
		- (m.m[0][2] * m.m[1][0] * m.m[3][1])
		+ (m.m[0][2] * m.m[1][1] * m.m[3][0])
		+ (m.m[0][1] * m.m[1][0] * m.m[3][2])
		+ (m.m[0][0] * m.m[1][2] * m.m[3][1]));

	result.m[3][3] = (1 / MatrixFormula) * (
		+(m.m[0][0] * m.m[1][1] * m.m[2][2])
		+ (m.m[0][1] * m.m[1][2] * m.m[2][0])
		+ (m.m[0][2] * m.m[1][0] * m.m[2][1])
		- (m.m[0][2] * m.m[1][1] * m.m[2][0])
		- (m.m[0][1] * m.m[1][0] * m.m[2][2])
		- (m.m[0][0] * m.m[1][2] * m.m[2][1]));


	return result;
}



//透視投影行列(正規化する)
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {};
	float theta = fovY / 2.0f;

	result.m[0][0] = (1.0f / aspectRatio) * Cot(theta);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = Cot(theta);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = farClip/ (farClip- nearClip);
	result.m[2][3] = 1;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = ( - nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0;

	return result;
}


//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float neaCrlip, float farClip) {
	Matrix4x4 result = {};
	result.m[0][0] = 2.0f /( right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 2.0f /( top - bottom);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = (1 / farClip - neaCrlip);
	result.m[2][3] = 0;

	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = neaCrlip / (neaCrlip - farClip);
	result.m[3][3] = 1;

	return result;
}

//MT4
Matrix3x3 MakeCrossMatrix(Vector3 v1, Vector3 v2) {
	Matrix3x3 a = {};
	a.m[0][0] = v1.x;
	a.m[0][1] = v1.y;
	a.m[0][2] = v1.z;

	a.m[1][0] = 0.0f;
	a.m[1][1] = 0.0f;
	a.m[1][2] = 0.0f;

	a.m[2][0] = 0.0f;
	a.m[2][1] = 0.0f;
	a.m[2][2] = 0.0f;


	Matrix3x3 b = {};
	b.m[0][0] = 0.0f;
	b.m[0][1] = -v2.z;
	b.m[0][2] = v2.y;

	b.m[1][0] = v2.z;
	b.m[1][1] = 0.0f;
	b.m[1][2] = -v2.x;

	b.m[2][0] = -v2.y;
	b.m[2][1] = v2.x;
	b.m[2][2] = 0.0f;


	//掛け算3x3
	Matrix3x3 result = {};
	result.m[0][0] = a.m[0][0] * b.m[0][0];
	result.m[0][1] = a.m[0][0] * b.m[0][0];
	result.m[0][2] = a.m[0][0] * b.m[0][0];

	result.m[1][0] = a.m[0][0] * b.m[0][0];
	result.m[1][1] = a.m[0][0] * b.m[0][0];
	result.m[1][2] = a.m[0][0] * b.m[0][0];

	result.m[2][0] = a.m[0][0] * b.m[0][0];
	result.m[2][1] = a.m[0][0] * b.m[0][0];
	result.m[2][2] = a.m[0][0] * b.m[0][0];

	


	return result;

}



//任意軸回転行列の作成関数
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {


	//angle...θ

#pragma region ロドリゲスの中身
	//r'・・回転後の点
	//p..ProjnR rからnへの射影ベクトル
	//a...r-p
	//b..aと長さが同じで垂直をなす回転面上のベクトル
	//c..r'-p

	//nはpの単位ベクトル
	//p=(r・n)n
	//Vector3 n = Normalize(axis);
	//float dotP=DotVector3(axis, n);
	//

	////p=(r・n)n
	//Vector3 p = { n.x * dotP,n.y * dotP,n.z * dotP };



	//Vector3 a = Subtract(axis, p);
	//Vector3 b = Cross(n, a);

	//Vector3 c = {
	//	a.x * std::cosf(angle) + b.x * std::sinf(angle),
	//	a.y * std::cosf(angle) + b.y * std::sinf(angle),
	//	a.z * std::cosf(angle + b.z * std::sinf(angle))
	//};


	//Vector3 rotatedVectorOrigin = {};
	//rotatedVectorOrigin = Add(p, c);



#pragma endregion

	Vector3 normalizeP = Normalize(axis);
	Vector3 projectR = Project(axis, normalizeP);
	

	//ロドリゲスの回転公式
	// nはRの単位ベクトル
	//R'=r*cosθ+(1-cosθ)projnR+(n×R)sinθ
	Vector3 rotatedVector = {
		axis.x*std::cosf(angle) +(1-std::cosf(angle))*projectR.x + Cross(normalizeP,axis).x*std::sinf(angle),
		axis.y*std::cosf(angle) +(1-std::cosf(angle))*projectR.y + Cross(normalizeP,axis).y*std::sinf(angle),
		axis.z*std::cosf(angle) +(1-std::cosf(angle))*projectR.z + Cross(normalizeP,axis).z*std::sinf(angle),
	};

#pragma region 01_01


	//rRにまとめたい
	//最終的にはrR=rS+rP+rC=r(S+P+C)という形


	//ベクトルでは
	//rR=r*cosθ
	//拡縮の行列にすればいいだけ
	//単位行列を掛けてね
	Matrix3x3 E = MakeIdentity3x3();
	Matrix3x3 S = MultiplyValueMatrix(std::cosf(angle),E);
	

	//P=(1-cosθ)projnR
	Matrix3x3 tBB = {};
	tBB.m[0][0] = normalizeP.x*normalizeP.x;
	tBB.m[0][1] = normalizeP.x*normalizeP.y;
	tBB.m[0][2] = normalizeP.x*normalizeP.z;

	tBB.m[1][0] = normalizeP.y*normalizeP.x;
	tBB.m[1][1] = normalizeP.y*normalizeP.y;
	tBB.m[1][2] = normalizeP.y*normalizeP.z;

	tBB.m[2][0] = normalizeP.z*normalizeP.x;
	tBB.m[2][1] = normalizeP.z*normalizeP.y;
	tBB.m[2][2] = normalizeP.z*normalizeP.z;

	float t = 1.0f/(Length(axis) * Length(axis));
	Matrix3x3 afterTBB = MultiplyValueMatrix(t, tBB);
	Matrix3x3 P = MultiplyVectorMatrix(axis, afterTBB);


	Matrix3x3 C = {};
	C.m[0][0] = 0.0f;
	C.m[0][1] = -normalizeP.z;
	C.m[0][2] = normalizeP.y;

	C.m[0][0] = normalizeP.z;
	C.m[0][1] = 0.0f;
	C.m[0][2] = -normalizeP.x;

	C.m[0][0] = -normalizeP.y;
	C.m[0][1] = normalizeP.x;
	C.m[0][2] = 0.0f;


#pragma endregion

	Vector4 newAxis = {axis.x,axis.y,axis.z,0.0f};

	Matrix4x4 resultR = {};
	resultR.m[0][0] = normalizeP.x * normalizeP.x * (1 - std::cosf(angle)) + std::cosf(angle);
	resultR.m[0][1] = normalizeP.x * normalizeP.y * (1 - std::cosf(angle)) + normalizeP.z*std::sinf(angle);
	resultR.m[0][2] = normalizeP.x * normalizeP.z * (1 - std::cosf(angle)) - normalizeP.y*std::sinf(angle);
	resultR.m[0][3] = 0.0f;

	resultR.m[1][0] = normalizeP.y * normalizeP.x * (1 - std::cosf(angle)) - normalizeP.z*std::sinf(angle);
	resultR.m[1][1] = normalizeP.y * normalizeP.y * (1 - std::cosf(angle)) + std::cosf(angle);
	resultR.m[1][2] = normalizeP.y * normalizeP.z * (1 - std::cosf(angle)) + normalizeP.y*std::sinf(angle);
	resultR.m[1][3] = 0.0f;

	resultR.m[2][0] = normalizeP.z * normalizeP.x * (1 - std::cosf(angle)) + normalizeP.y*std::sinf(angle);
	resultR.m[2][1] = normalizeP.z * normalizeP.y * (1 - std::cosf(angle)) - normalizeP.x*std::sinf(angle);
	resultR.m[2][2] = normalizeP.z * normalizeP.z * (1 - std::cosf(angle)) +std::cosf(angle);
	resultR.m[2][3] = 0.0f;

	resultR.m[3][0] = 0.0f;
	resultR.m[3][1] = 0.0f;
	resultR.m[3][2] = 0.0f;
	resultR.m[3][3] = 1.0f;

	

	return resultR;

}

//ある方向からある方向への回転
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
	
	Vector3 normalizeFrom = Normalize(from);
	Vector3 normalizeTo = Normalize(to);


	Vector3 uvCross = Cross(normalizeFrom, normalizeTo);
	Vector3 n = Normalize(uvCross);

	//内積で求めてね
	float cos = DotVector3(normalizeFrom,normalizeTo);

	//外積で求めてね
	float sin = Length(Cross(normalizeFrom,normalizeTo));

	//形自体は前回とほぼ同じ
	//見比べてみよう！
	Matrix4x4 resultR = {};
	resultR.m[0][0] = n.x * n.x * (1 - cos) + cos;
	resultR.m[0][1] = n.x * n.y * (1 - cos) + n.z*sin;
	resultR.m[0][2] = n.x * n.z * (1 - cos) - n.y*sin;
	resultR.m[0][3] = 0.0f;

	resultR.m[1][0] = n.y * n.x * (1 - cos) - n.z*sin;
	resultR.m[1][1] = n.y * n.y * (1 - cos) + cos;
	resultR.m[1][2] = n.y * n.z * (1 - cos) + n.x*sin;
	resultR.m[1][3] = 0.0f;

	resultR.m[2][0] = n.z * n.x * (1 - cos) + n.y*sin;
	resultR.m[2][1] = n.z * n.y * (1 - cos) - n.x*sin;
	resultR.m[2][2] = n.z * n.z * (1 - cos) + cos;
	resultR.m[2][3] = 0.0f;

	resultR.m[3][0] = 0.0f;
	resultR.m[3][1] = 0.0f;
	resultR.m[3][2] = 0.0f;
	resultR.m[3][3] = 1.0f;

	return resultR;
}
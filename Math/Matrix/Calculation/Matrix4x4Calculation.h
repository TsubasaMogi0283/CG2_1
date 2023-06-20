#pragma once
#include "Matrix/Matrix/Matrix4x4.h"
#include "Vector/Vector3.h"


#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


//クロス積
Vector3 Cross(const Vector3 v1, const Vector3 v2);

//コタンジェント
float Cot(float theta);

//単位行列を作成する
Matrix4x4 MakeIdentity4x4();

//乗算
Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2);

//Scale
//拡縮
Matrix4x4 MakeScaleMatrix(const Vector3 scale);

Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);

Matrix4x4 MakeRotateZMatrix(float radian);

//AllRotate
//回転
Matrix4x4 MakeRotateXYZMatrix(float radianX, float radianY, float radianZ);


//Translate
//移動
Matrix4x4 MakeTranslateMatrix(Vector3 translate);



//AffineMatrix
//SRTの融合
//アフィン行列
Matrix4x4 MakeAffineMatrix(const Vector3 scale, const Vector3 rotate, const Vector3 translate);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
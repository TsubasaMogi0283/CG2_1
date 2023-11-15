#pragma once
#include <Math/Vector/Vector3.h>
#include <Math/Matrix/Matrix/Matrix4x4.h>

//Vector3

//足し算
Vector3 Add(Vector3 v1, Vector3 v2);

//引き算
Vector3 Subtract(Vector3 v1, Vector3 v2);

//回転
Vector3 TransformNormal(Vector3 v, Matrix4x4 m);

Vector3 Normalize(Vector3 V1);

//線形補間
Vector3 Leap(const Vector3& v1, const Vector3& v2, float t);

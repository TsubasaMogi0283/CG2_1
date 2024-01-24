#include "VectorCalculation.h"

#include "VectorCalculation.h"
#include <corecrt_math.h>
#include <cmath>
#include <iostream>
#include <numbers>

Vector3 Add(Vector3 v1, Vector3 v2) {
	Vector3 result = {};
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

//引き算
Vector3 Subtract(Vector3 v1, Vector3 v2) {
	Vector3 result = {};
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}


Vector3 TransformNormal(Vector3 v, Matrix4x4 m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};


	return result;
}

Vector3 Normalize(Vector3 V1) {
	Vector3 result = {};

	float length = sqrtf(V1.x * V1.x + V1.y * V1.y + V1.z * V1.z);

	float newX = V1.x;
	float newY = V1.y;
	float newZ = V1.z;


	if (length != 0.0f) {
		newX = V1.x / length;
		newY = V1.y / length;
		newZ = V1.z / length;

	}

	result.x = newX;
	result.y = newY;
	result.z = newZ;


	return result;
}

float Length(Vector3 v1) {
	float result = sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);

	return result;
}

Vector3 Leap(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result = {};

	if (t < 0.0f) {
		t = 0.0f;
	}
	if (t > 1.0) {
		t = 1.0f;
	}

	result.x = v1.x + t * (v2.x - v1.x);
	result.y = v1.x + t * (v2.y - v1.y);
	result.z = v1.z + t * (v2.z - v1.z);


	return result;
}

float Clamp(float t, float min, float max) {
	if (t < min) {
		return min;
	}
	else if (t > max) {
		return max;
	}

	return t;


}

float DotVector3(Vector3 v1, Vector3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float DotVector2(Vector3 v1, Vector3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	float newT = Clamp(t, 0.0f, 1.0f);

	Vector3 normalizeV1 = Normalize(v1);
	Vector3 normalizeV2 = Normalize(v2);


	float dot = DotVector3(normalizeV1, normalizeV2);

	float theta = std::acosf(dot) * newT;

	Vector3 subtractVector3 = Subtract(v2, v1);
	Vector3 relativeVector = Normalize(
		{ subtractVector3.x * newT,
		subtractVector3.y * newT,
		subtractVector3.z * newT });

	Vector3 result = {
		v1.x * std::cos(theta) + relativeVector.x * std::sin(theta),
		v1.y * std::cos(theta) + relativeVector.y * std::sin(theta),
		v1.z * std::cos(theta) + relativeVector.z * std::sin(theta)
	};

	return result;
}

//Catmull-romスプライン曲線
Vector3 CatmullRom3D(std::vector<Vector3> controlPoint, float t) {
	/*Vector3 result = {};

	float half = 1.0f / 2.0f;
	for (int i = 0; i < 3; i++) {
		Vector3 p0 = controlPoint[i];
		Vector3 p1 = controlPoint[i + 3];
		Vector3 p2 = controlPoint[i + 6];
		Vector3 p3 = controlPoint[i + 9];



		result.x = float(half * (
			(-1.0f * -p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * std::pow(t, 3) +
			(2.0f * p0.x - 5.0f * p1.x - 4.0f * p2.x - p3.x) * std::pow(t, 2) +
			(-1.0f * p0.x + p2.x * t) + 2.0f * p0.x));

		result.y = float(half * (
			(-1.0f * -p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * std::pow(t, 3) +
			(2.0f * p0.y - 5.0f * p1.y - 4.0f * p2.y - p3.y) * std::pow(t, 2) +
			(-1.0f * p0.y + p2.y * t) + 2.0f * p0.y));

		result.z = float(half * (
			(-1.0f * -p0.z + 3.0f * p1.z - 3.0f * p2.z + p3.z) * std::pow(t, 3) +
			(2.0f * p0.z - 5.0f * p1.z - 4.0f * p2.z - p3.z) * std::pow(t, 2) +
			(-1.0f * p0.z + p2.z * t) + 2.0f * p0.z));

	}



	return result;*/


	//クラモト君の参考にさせてもらった
	//いずれ自分なりに変えるつもりではある 

	//要素数を取得する
	//size関数は要素数を返す　例　std::vector<int>sanple = {1, 2, 3, } の時 sanple.size() = 3
	int elements = static_cast<int> (controlPoint.size());
	//tの値x(要素数-1) 0からスタートにしたいので、-1する
	//少数点以下を切り捨てるためにint型にする
	//線分の位置を特定する
	int segment = static_cast<int> (t * (elements - 1));
	//小数点以下を取り出すためにfloat型segment-int型segmentをする
	//線分の位置からtを0~1の範囲に収める
	float tSegment = t * (elements - 1) - segment;
	//始点
	//線分の位置が1以上なら線分の現在地からひとつ前を代入　線分の位置が0以下なら0を代入
	Vector3 p0;
	if (segment > 0) {
		p0 = controlPoint[segment - 1];
	}
	else {
		p0 = controlPoint[0];
	}
	//始点から伸びるベクトル
	//要素数-1より線分が大きくなったら、制御点から飛び出てしまうので要素数-1を代入
	Vector3 v0;
	if (segment < elements - 1) {
		v0 = controlPoint[segment + 1];
	}
	else {
		v0 = controlPoint[elements - 1];
	}
	//始点の一つ先の制御点
	Vector3 p1 = controlPoint[segment];
	//始点の一つ先の制御点から伸びるベクトル
	//要素数-2より線分が大きくなったら、制御点から飛び出てしまうので要素数-1を代入
	Vector3 v1;
	if (segment < elements - 2) {
		v1 = controlPoint[segment + 2];
	}
	else {
		v1 = controlPoint[elements - 1];
	}
	Vector3 result;
	//エルミート曲線の式に、全ての制御点を通るように変更を加えた式
	result.x = 0.5f *
		(((-p0.x + 3.0f * p1.x - 3.0f * v0.x + v1.x) * (tSegment * tSegment * tSegment)) +
			((2.0f * p0.x - 5.0f * p1.x + 4.0f * v0.x - v1.x) * (tSegment * tSegment)) +
			((2.0f * p1.x) + (-p0.x + v0.x) * tSegment));
	result.y = 0.5f *
		(((-p0.y + 3.0f * p1.y - 3.0f * v0.y + v1.y) * (tSegment * tSegment * tSegment)) +
			((2.0f * p0.y - 5.0f * p1.y + 4.0f * v0.y - v1.y) * (tSegment * tSegment)) +
			((2.0f * p1.y) + (-p0.y + v0.y) * tSegment));
	result.z = 0.5f *
		(((-p0.z + 3.0f * p1.z - 3.0f * v0.z + v1.z) * (tSegment * tSegment * tSegment)) +
			((2.0f * p0.z - 5.0f * p1.z + 4.0f * v0.z - v1.z) * (tSegment * tSegment)) +
			((2.0f * p1.z) + (-p0.z + v0.z) * tSegment));
	return result;
}

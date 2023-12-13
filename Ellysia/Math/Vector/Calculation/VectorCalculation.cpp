#include "VectorCalculation.h"
#include <cmath>

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

Vector3 TransformNormal(Vector3 v, Matrix4x4 m){
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	

	return result;
}

Vector3 Normalize(Vector3 V1) {
	Vector3 result = {};

	float length = sqrtf(V1.x * V1.x + V1.y * V1.y+V1.z*V1.z);

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

float Length(Vector3 v1){
	float result = sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
	
	return result;
}

Vector3 Leap(const Vector3& v1, const Vector3& v2, float t){
	Vector3 result = {};

	result.x = v1.x + t*(v2.x - v1.x) ;
	result.y = v1.x + t*(v2.y - v1.y) ;
	result.z = v1.z + t*(v2.z - v1.z) ;


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

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t){
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
		v1.x*std::cos(theta)+relativeVector.x*std::sin(theta),
		v1.y*std::cos(theta)+relativeVector.y*std::sin(theta),
		v1.z*std::cos(theta)+relativeVector.z*std::sin(theta)
	};

	return result;
}

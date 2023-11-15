#include "VectorCalculation.h"
#include <corecrt_math.h>

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

#include "Matrix/Calculation/Matrix4x4Calculation.h"

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
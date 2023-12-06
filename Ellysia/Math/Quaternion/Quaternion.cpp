#include "Quaternion.h"
#include <cmath>

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs){
    Quaternion result = {};
    
    return Quaternion();
}

Quaternion IdentityQuaternion(){
    Quaternion result = {};





    return result;
}

Quaternion Conjugate(const Quaternion& quaternion){
    return Quaternion();
}

float Norm(const Quaternion& quaternion){
    return 0.0f;
}

Quaternion Normalize(const Quaternion& quaternion){
    Quaternion result = {};
    

    float length = sqrtf(
        quaternion.x * quaternion.x + 
        quaternion.y * quaternion.y+
        quaternion.z*quaternion.z+
        quaternion.w*quaternion.w
    );


    Quaternion preResult = {};
    preResult.x = quaternion.x;
	preResult.y = quaternion.y;
	preResult.z = quaternion.z;
    preResult.w = quaternion.w;

	if (length != 0.0f) {
		preResult.x = quaternion.x / length;
		preResult.y = quaternion.y / length;
		preResult.z = quaternion.z / length;
        preResult.w = quaternion.w / length;

	}

	result.x = preResult.x;
	result.y = preResult.y;
	result.z = preResult.z;
    result.w = preResult.w;


    return result;
}

Quaternion Inverse(const Quaternion& quaternion){

    return Quaternion();
}

#include "Quaternion.h"
#include <cmath>

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs){
    Quaternion result = {};
    Quaternion q = lhs;
    Quaternion r = rhs;
    
    
    //wはそのまま
    //xはi部分
    //yはj部分
    //zはk部分

    result.w = q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;
    result.x = q.y * r.z - q.z * r.y + r.w * q.x + q.w * r.x;
    result.y = q.z * r.x - q.x * r.z + r.w * q.y + q.w * r.y;
    result.z = q.x * r.y - q.y * r.x + r.w * q.z + q.w * r.z;
    


    return result;
}

//単位
Quaternion IdentityQuaternion(){

    //e=1+0i+0j+0k
    //qe=eq=q

    Quaternion result = {};

    result.w = 1.0f;
    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;
    




    return result;
}

//共役
Quaternion Conjugate(const Quaternion& quaternion){
    
    //q*=w-xi-yj-zk=(qw,-qv)
    //虚数部分があるところはマイナスになるよ
    Quaternion result = {};
    result.x = -quaternion.x;
    result.y = -quaternion.y;
    result.z = -quaternion.z;
    result.w = quaternion.w;


    return result;
}

float Norm(const Quaternion& quaternion){

    //||q||=sqrtf(qq*)
    float result = 0.0f;
    result = sqrtf(
        quaternion.w * quaternion.w +
        quaternion.x * quaternion.x +
        quaternion.y * quaternion.y +
        quaternion.z * quaternion.z);
     
    return result;
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
    //q^-1=q*
    //   ||q||^2


    Quaternion result = {};


    float norm = Norm(quaternion);
    Quaternion conjugate = Conjugate(quaternion);

    float t = norm * norm;

    result.x = conjugate.x / t;
    result.y = conjugate.y / t;
    result.z = conjugate.z / t;
    result.w = conjugate.w / t;


    return result;
}

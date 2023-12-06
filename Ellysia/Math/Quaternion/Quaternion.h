#pragma once

//四元数
//回転する為だけにに使われる
struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};


//q=w+xi+yj+zk
//i^2=j^2=k^2=ijk=-1
//ij=k,jk=i,ki=j,
//ji=-k,kj=-i,ik=-j


//q=(qw,qv)
//qw=w
//qv=(x,y,z)

//Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

//単位のQuaternionを返す
Quaternion IdentityQuaternion();

//共役Quaternionを返す
Quaternion Conjugate(const Quaternion& quaternion);

//Quaternionのnormを返す
float Norm(const Quaternion& quaternion);

//正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion);

//逆Quaternionを返す
Quaternion Inverse(const Quaternion& quaternion);


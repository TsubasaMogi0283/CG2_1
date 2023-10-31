#pragma once
#include "Common/Windows/WinApp.h"

#include <Math/Vector/Transform.h>
#include <Math/Matrix/Matrix/Matrix4x4.h>

class Camera {
private:
	//コンストラクタ
	Camera();

	//デストラクタ
	~Camera();


public:
	static Camera* GetInstance();



	//コピーコンストラクタ禁止
	Camera(const Camera& camera) = delete;

	//代入演算子を無効にする
	Camera& operator=(const Camera& camera) = delete;


public:

	//基本はアクセッサだけにする
	void SetRotate(Vector3 rotate) {
		this->cameraTransform_.rotate = rotate;
	}

	void SetTranslate(Vector3 translate) {
		this->cameraTransform_.translate = translate;
	}

	Matrix4x4 GetViewMatrix();

	Matrix4x4 GetProjectionMatrix_();

private:
	static Camera* instance_;

	Matrix4x4 cameraMatrix_ = {};
	Matrix4x4 viewMatrix_ = {};

	//遠視投影行列
	Transform cameraTransform_ = {};
	Matrix4x4 projectionMatrix_ = {};

};

#pragma once

#include "Transform.h"
#include "Matrix4x4.h"

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

	Vector3 GetRotate() {
		return cameraTransform_.rotate;
	}
	Vector3 GetTranslate() {
		return cameraTransform_.translate;
	}

	void Initialize();

	Matrix4x4 GetAffineMatrix();

	Matrix4x4 GetViewMatrix();

	void SetViewMatrix(Matrix4x4 matrix) {
		this->viewMatrix_ = viewMatrix_;
	}

	Matrix4x4 GetProjectionMatrix_();



private:
	static Camera* instance_;

	Matrix4x4 cameraMatrix_ = {};
	Matrix4x4 viewMatrix_ = {};

	//遠視投影行列
	Transform cameraTransform_ = {};
	Matrix4x4 projectionMatrix_ = {};

};

#pragma once
#include "Common/DirectX/DirectXSetup.h"

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


	//デリート代わりの関数
	void DeleteInstance();

	//コピーコンストラクタ禁止
	Camera(const Camera& camera) = delete;

	//代入演算子を無効にする
	Camera& operator=(const Camera& camera) = delete;


public:

	void Initialize();

	void Update();


	void SetRotate(Vector3 rotate) {

	}

	void SetTranslate(Vector3 translate) {

	}

private:
	static Camera* instance_;



	Matrix4x4 cameraMatrix_ = {};
	Matrix4x4 viewMatrix_ = {};

	//遠視投影行列
	Transform cameraTransform_ = {};
	Matrix4x4 projectionMatrix_ = {};

};

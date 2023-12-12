#pragma once
#include "DirectXSetup.h"
#include <DirectionalLight.h>

class CreateDirectionalLight{
public:

	void Initialize();


	void SetGraphicsCommand();

	
#pragma region アクセッサ
	Vector3 GetDirection() {
		return direction_;
	}
	void SetDirection(Vector3 direction) {
		this->direction_ = direction;
	}

	void SetColor(Vector4 color) {
		this->color_ = color;
	}
	void SetIntensity(float intensity) {
		this->intensity_ = intensity;
	}

#pragma endregion

private:
	ComPtr<ID3D12Resource> resource_ = nullptr;
	DirectionalLight* data_ = nullptr;


	Vector3 direction_ = {};
	Vector4 color_ = {};
	float intensity_ = {};

};



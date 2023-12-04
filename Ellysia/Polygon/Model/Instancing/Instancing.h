#pragma once
#include "Common/DirectX/DirectXSetup.h"

#include "Math/Vector/Transform.h"
#include <Math/Matrix/Matrix/TransformationMatrix.h>

class Instancing{
public:
	//コンストラクタ
	Instancing();

	//初期化
	void Initialize();

	//グラフィックコマンドの設定
	void SetGraphicsCommand();

	//ドローコール
	void GraphicsCommand();

	//デストラクタ
	~Instancing();

public:
	int GetInstanceCount() {
		return instanceCount_;
	}

private:
	
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_ = {};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_ = {};

	ComPtr<ID3D12Resource>instancingResource_ = nullptr;

	static const int32_t instanceCount_ = 10;
	Transform transforms[instanceCount_];
	TransformationMatrix* instancingData_ = nullptr;
};


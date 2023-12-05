#pragma once
#include "Common/DirectX/DirectXSetup.h"

#include "Math/Vector/Transform.h"
#include <Math/Matrix/Matrix/TransformationMatrix.h>
#include "Polygon/Particle/3D/Particle.h"

#include <random>

class Instancing{
public:
	//コンストラクタ
	Instancing();

	//初期化
	void Initialize(std::mt19937& randomEngine);

	//グラフィックコマンドの設定
	void SetGraphicsCommand();

	//ドローコール
	void GraphicsCommand();

	//デストラクタ
	~Instancing();

public:
	int GetInstanceCount() {
		return MAX_INSTANCE_NUMBER_;
	}

	uint32_t GetCurrentInstanceNumber() {
		return numInstance_;
	}

private:
	
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_ = {};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_ = {};

	ComPtr<ID3D12Resource>instancingResource_ = nullptr;

	static const int32_t MAX_INSTANCE_NUMBER_ = 10;
	//描画すべきインスタンス数
	uint32_t numInstance_ = 0;
	//パーティクル
	Particle particles_[MAX_INSTANCE_NUMBER_];
	ParticleForGPU* instancingData_ = nullptr;
};

#pragma once
#include "Common/DirectX/DirectXSetup.h"

#include "Math/Vector/Transform.h"
#include <Math/Matrix/Matrix/TransformationMatrix.h>
#include "Polygon/Particle/3D/Particle.h"

#include <random>
#include <Polygon/Model/Mesh/Mesh.h>


//インスタンシングクラスとメッシュクラス一緒にする
class Instancing{
public:
	//コンストラクタ
	Instancing();

	//初期化
	void Initialize(std::mt19937& randomEngine,const std::vector<VertexData>& vertices);


	//グラフィックコマンドの設定
	void SetGraphicsCommand();

	//デストラクタ
	~Instancing();



private:
	//ドローコール
	void DrawCall();

	
public:
	int GetInstanceCount() {
		return MAX_INSTANCE_NUMBER_;
	}

	uint32_t GetCurrentInstanceNumber() {
		return numInstance_;
	}

	void SetIsBillBordMode(bool isBillBordMode) {
		this->isBillBordMode_ = isBillBordMode;
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


	//メッシュクラスのやつ
	//頂点リソースを作る
	std::unique_ptr<Mesh> mesh_ = nullptr;

	//基本オンにしておく
	bool isBillBordMode_ = true;

};


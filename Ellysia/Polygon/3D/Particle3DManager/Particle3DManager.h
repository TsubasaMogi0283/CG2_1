#pragma once
#include <unordered_map>
#include <string>

#include "DirectXSetup.h"
#include "MaterialData.h"
#include <Particle.h>
#include "VertexData.h"

struct ParticleGrounp {
	//マテリアルデータ
	MaterialData materialData;
	//テクスチャ用SRVインデックス
	uint32_t textureSrvindex;

	//パーティクルのリスト
	std::list<Particle>particles;

	//インスタンシングデータ用SRVインデックス
	uint32_t instancingSrvIndex;
	//インスタンシングリソース
	ComPtr<ID3D12Resource>instancingResource;
	//インスタンス数
	uint32_t instanceNumber;
	//インスタンシングデータに書き込むためのポインタ
	ParticleForGPU* instancingData;
};

class Particle3DManager final{
private:
	//コンストラクタ
	Particle3DManager() = default;

	//デストラクタ
	~Particle3DManager() = default;

public:
	//インスタンス
	static Particle3DManager* GetInstance();

	//コピーコンストラクタ禁止
	Particle3DManager(const Particle3DManager& particle3DManager) = delete;

	//代入演算子を無効にする
	Particle3DManager& operator=(const Particle3DManager& particle3DManager) = delete;


public:
	void Initialize(uint32_t modelHandle);



	void CreateParticleGroup(const std::string name, uint32_t textureHandle);



private:
	//ユーザーが付けるグループ名をキーとしてグループを持てるようにする
	std::unordered_map<std::string, ParticleGrounp>particleGroup_;

	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	std::vector<VertexData> vertices_{};

	
	const int MAX_INSTANCE_NUMBER_ = 100;
};


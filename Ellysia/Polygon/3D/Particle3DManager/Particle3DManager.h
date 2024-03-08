#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include <random>

#include "DirectXSetup.h"
#include "MaterialData.h"
#include <Particle.h>
#include "VertexData.h"
#include <Camera.h>
#include <AccelerationField.h>
#include <DirectionalLight.h>

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

struct Emitter {
	//エミッタのTransform;
	Transform transform;
	//発生数
	uint32_t count;
	//発生頻度
	float frequency;
	//頻度用時刻
	float frequencyTime;
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

	void Emit(const std::string name, const Vector3& position, uint32_t count);

	void Draw(Camera& camera, uint32_t textureHandle);


private:
	Particle MakeNewParticle(std::mt19937& randomEngine);

	std::list<Particle> Emission(const Emitter& emmitter, std::mt19937& randomEngine);

	void Update(Camera& camera);

private:
	//ユーザーが付けるグループ名をキーとしてグループを持てるようにする
	std::unordered_map<std::string, ParticleGrounp>particleGroup_;

	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	std::vector<VertexData> vertices_{};

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	//色関係のメンバ変数
	Vector4 materialColor_ = { 1.0f,1.0f,1.0f,1.0f };

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	ComPtr<ID3D12Resource>instancingResource_ = nullptr;

	//Lighting用
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	//色
	Vector4 directionalLightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	float directionalLightIntensity_ = 3.0f;

	//基本はtrueで
	bool isEnableLighting_ = true;
	//方向
	Vector3 lightingDirection_ = { 0.0f,-1.0f,0.0f };

	
	const uint32_t  MAX_INSTANCE_NUMBER_ = 100;

	//描画すべきインスタンス数
	uint32_t numInstance_ = 0;
	bool isBillBordMode_ = true;

	//エミッタの設定
	Emitter emitter_ = {};
	const float DELTA_TIME = 1.0f / 60.0f;

	//フィールド
	bool isSetField_ = false;
	AccelerationField accelerationField_ = {};


	//パーティクル
	std::list<Particle>particles_;
	ParticleForGPU* instancingData_ = nullptr;
};


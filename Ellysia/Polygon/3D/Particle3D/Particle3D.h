#pragma once
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <array>
#include <memory>
#include <list>
#include <random>

#include <DirectXTeX.h>
#include "Matrix4x4.h"
#include <Material.h>
#include <TransformationMatrix.h>
#include <DirectionalLight.h>
#include "MaterialData.h"
#include "ModelData.h"
#include "Camera.h"

#include "Vector4.h"
#include "Matrix4x4Calculation.h"
#include <VertexData.h>
#include "WorldTransform.h"

#include <VectorCalculation.h>
#include <d3dx12.h>




#include "Mesh.h"
#include "CreateMaterial.h"
#include "CreateDirectionalLight.h"
#include "Transformation.h"
#include "Transform.h"
#include "Particle.h"
#include <AccelerationField.h>


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

class Particle3D {
public:

	//コンストラクタ
	Particle3D();

	//初期化
	void Create(const std::string& directoryPath, const std::string& fileName);


private:
#pragma region モデルの読み込み関係の関数
	//モデルデータの読み込み
	ModelData LoadObjectFile(const std::string& directoryPath, const std::string& fileName);

	//mtlファイルの読み込み
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);



#pragma endregion

#pragma region パーティクルの設定で使う関数

	//パーティクルの初期化をする関数
	Particle MakeNewParticle(std::mt19937& randomEngine);

	//Emitterで発生させる
	std::list<Particle> Emission(const Emitter& emmitter, std::mt19937& randomEngine);

#pragma endregion

	void Update(Camera& camera);

public:



	//通常の描画
	//void Draw(Camera& camera);

	//テクスチャを上書きをする描画
	void Draw(Camera& camera, uint32_t textureHandle);


	//デストラクタ
	~Particle3D();




public:
	//アクセッサのまとめ

	
	void SetColor(Vector4 color) {
		this->color_ = color;
	}

	//透明度の変更
	void SetTransparency(float transparency) {
		this->color_.w = transparency;
	}

	//ビルボードにするかどうか
	//デフォルトではするようにしている
	bool IsBillBordMode(bool isBillBordMode) {
		this->isBillBordMode_ = isBillBordMode;
	}


#pragma region エミッタの中の設定


#pragma region SRT
	//Scale
	void SetScale(Vector3 scale) {
		this->emitter_.transform.scale = scale;
	}

	//Rotate
	void SetRotate(Vector3 rotate) {
		this->emitter_.transform.rotate = rotate;
	}
	Vector3 GetRotate() {
		return emitter_.transform.rotate;
	}

	//Translate
	void SetTranslate(Vector3 translate) {
		this->emitter_.transform.translate = translate;
	}
	Vector3 GetTranslate() {
		return emitter_.transform.translate;
	}

#pragma endregion

	//発生数
	void SetCount(uint32_t count) {
		this->emitter_.count = count;
	}
	//発生頻度
	void SetFrequency(float frequency) {
		this->emitter_.frequency = frequency;
	}
	//発生頻度を設定
	void SetFrequencyTime(float frequencyTime) {
		this->emitter_.frequencyTime = frequencyTime;
	}


	//以下の2つはセットで使ってね
	void SetField(bool isSetField) {
		this->isSetField_ = isSetField;
	}
	void SetAccelerationField(AccelerationField accelerationField) {
		this->accelerationField_ = accelerationField;
	}

#pragma endregion

#pragma region Lightingの設定
	void SetLighting(bool enableLighting) {
		this->isEnableLighting_ = enableLighting;
	}
	//方向
	void SetDirection(Vector3 direction) {
		this->lightingDirection_ = direction;
	}

#pragma endregion

private:
	//TextureManagerを参考にする
	std::list<ModelData> modelInformationList_;

	//頂点データ
	std::unique_ptr<Mesh> mesh_ = nullptr;
	std::unique_ptr<Transformation> transformation_ = nullptr;
	std::vector<VertexData> vertices;
	//マテリアル用のリソースを作る
	std::unique_ptr<CreateMaterial> material_ = nullptr;
	//色関係のメンバ変数
	Vector4 color_ = {};


	//Lighting用
	std::unique_ptr<CreateDirectionalLight> directionalLight_ = nullptr;
	//基本はtrueで
	bool isEnableLighting_ = true;
	//方向
	Vector3 lightingDirection_ = { 0.0f,-1.0f,0.0f };



#pragma region インスタンシング関係の変数

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_ = {};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_ = {};

	ComPtr<ID3D12Resource>instancingResource_ = nullptr;


	static const int32_t MAX_INSTANCE_NUMBER_ = 100;
	//描画すべきインスタンス数
	uint32_t numInstance_ = 0;
	//パーティクル
	//Particle particles_[MAX_INSTANCE_NUMBER_] = {};
	std::list<Particle>particles_;
	ParticleForGPU* instancingData_ = nullptr;

	//ビルボード
	bool isBillBordMode_ = true;

#pragma endregion

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;


	//エミッタの設定
	Emitter emitter_ = {};
	const float DELTA_TIME = 1.0f / 60.0f;

	//フィールド
	bool isSetField_ = false;
	AccelerationField accelerationField_ = {};

};
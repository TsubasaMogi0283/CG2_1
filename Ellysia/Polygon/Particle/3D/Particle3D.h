#pragma once
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <array>
#include <memory>
#include <list>
#include <random>

#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Matrix/Matrix4x4.h"
#include <Math/Vector/Material.h>
#include <Math/Matrix/Matrix/TransformationMatrix.h>
#include <Math/Vector/DirectionalLight.h>
#include "Polygon/Model/MaterialData/MaterialData.h"
#include "Polygon/Model/ModelData/ModelData.h"



#include "Vector4.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"
#include <Math/Vector/VertexData.h>
#include "Math/Matrix/Matrix/WorldViewMatrix.h"

#include <Math/Vector/Calculation/VectorCalculation.h>
#include <externals/DirectXTex/d3dx12.h>




#include "Polygon/Model/Mesh/Mesh.h"
#include "Polygon/Model/Material/CreateMaterial.h"
#include "Polygon/Model/DirectionalLight/CreateDirectionalLight.h"
#include "Polygon/Model/Transformation/Transformation.h"
#include "Transform.h"
#include "Particle.h"
#include "Emitter.h"


class Particle3D {
public:

	//コンストラクタ
	Particle3D();

	//初期化
	void CreateRandomParticle();

	//ファイルの読み込み
	void LoadObject(const std::string& directoryPath, const std::string& fileName);

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

public:

	void Update(std::mt19937 randomEngine, const std::string& directoryPath, const std::string& fileName);

	//通常の描画
	//void Draw();

	//テクスチャを上書きをする描画
	void Draw(uint32_t textureHandle_ );


	//デストラクタ
	~Particle3D();




public:
	//アクセッサのまとめ

	//透明度の変更
	void SetColor(Vector4 color) {
		this->color_ = color;
	}

	void SetTransparency(float transparency) {
		this->color_.w = transparency;
	}
	

	
	//アクセッサのまとめ
	//Scale
	void SetScale(Vector3 scale) {
		this->emitter_.transform.scale = scale;
	}

	//Rotate
	void SetRotate(Vector3 rotate) {
		this->emitter_.transform.rotate = rotate;
	}
	//Rotate
	void SetTranslate(Vector3 translate) {
		this->emitter_.transform.translate = translate;
	}


	//ビルボードにするかどうか
	bool IsBillBordMode(bool isBillBordMode) {
		this->isBillBordMode_ = isBillBordMode;
	}


#pragma region エミッタの中の設定
	
	void SetEmitter(Emitter emitter) {
		this->emitter_ = emitter;
	}

	//発生数
	void SetCount(uint32_t count) {
		this->emitter_.count = count;
	}
	//発生頻度
	void SetFrequency(float frequency){
		this->emitter_.frequency = frequency;
	}
	//発生頻度を設定
	void SetFrequencyTime(float frequencyTime){
		this->emitter_.frequencyTime = frequencyTime;
	}

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
	std::string directoryPath_;
	std::string fileName_;

	//頂点データ
	std::unique_ptr<Mesh> mesh_ = nullptr;
	std::unique_ptr<Transformation> transformation_= nullptr ;
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
	Vector3 lightingDirection_ = {0.0f,-1.0f,0.0f};




	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_ = {};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_ = {};

	ComPtr<ID3D12Resource>instancingResource_ = nullptr;

	static const int32_t MAX_INSTANCE_NUMBER_ = 10;
	//描画すべきインスタンス数
	uint32_t numInstance_ = 0;
	//パーティクル
	//Particle particles_[MAX_INSTANCE_NUMBER_] = {};
	std::list<Particle>particles_;
	ParticleForGPU* instancingData_ = nullptr;

	//ビルボード
	bool isBillBordMode_ = true;
	//SRT
	Vector3 scale_ = { 1.0f,1.0f,1.0f };
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };
	Vector3 translate_ = { 0.0f,0.0f,0.0f };



	//テクスチャハンドル
	uint32_t textureHandle_ = 0;


	//エミッタの設定
	Emitter emitter_ = {};
	const float DELTA_TIME = 1.0f / 60.0f;

};

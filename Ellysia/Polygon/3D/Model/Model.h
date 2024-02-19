#pragma once
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <array>
#include <memory>
#include <DirectXTex.h>
#include <d3dx12.h>

#include "Matrix4x4.h"
#include "Vector4.h"
#include "TransformationMatrix.h"
#include "Matrix4x4Calculation.h"
#include "VertexData.h"


#include "MaterialData.h"
#include "ModelData.h"
#include "DirectionalLight.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "LoadModelData.h"

enum LightingKinds {
	None,
	Directional,

};

class Model {
public:

	//コンストラクタ
	Model();

	//初期化
	//Initializeも兼ねているよ
	//通常
	static Model* Create(const std::string& directoryPath,const std::string& fileName);
private:
#pragma region モデルの読み込み関係の関数
	
#pragma endregion

public:
	//描画
	void Draw(WorldTransform& worldTransform, Camera& camera);

	//デストラクタ
	~Model();




public:



	
	//透明度の変更
	void SetColor(Vector4 color) {
		this->materialColor_ = color;
	}

	void SetTransparency(float transparency) {
		this->materialColor_.w = transparency;
	}


	//アクセッサのまとめ
	void SetBlendMode(int32_t blendmode) {
		blendModeNumber_ = blendmode;
	}

#pragma region Lightingの設定
	void SetLighting(bool enableLighting) {
		this->isEnableLighting_ = enableLighting;
	}
	//方向
	void SetDirectionalLightDirection(Vector3 direction) {
		this->lightingDirection_ = direction;
	}
	//Intensity
	void SetDirectionalLightIntensity(float intensity) {
		this->directionalLightIntensity_ = intensity;
	}

#pragma endregion

private:
	struct Material {
		Vector4 color;
		int32_t lightingKinds;
		float padding[3];
		Matrix4x4 uvTransform;
		float shininess;
	};


private:
	//頂点リソースを作る
	//頂点バッファビューを作成する
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	std::vector<VertexData> vertices_{};



	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;
	//色関係のメンバ変数
	Vector4 materialColor_ = { 1.0f,1.0f,1.0f,1.0f };


	//Lighting用
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;
	//基本はtrueで
	int32_t isEnableLighting_ = Directional;
	//方向
	Vector3 lightingDirection_ = {0.0f,-1.0f,0.0f};
	//Lightの色
	Vector4 lightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	float directionalLightIntensity_ = 2.0f;
	float shininess_ = 100.0f;

	//PixelShaderにカメラの座標を送る為の変数
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	CameraForGPU* cameraForGPU_ = {};

	uint32_t textureHandle_ = 0;


	//TextureManagerを参考にする
	static std::list<ModelData> modelInformationList_;


	//デフォルトはα加算
	int32_t blendModeNumber_ = 1;;

};
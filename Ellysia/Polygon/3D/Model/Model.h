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
#include "Transform.h"
#include "Material.h"
#include "TransformationMatrix.h"

#include "Matrix4x4Calculation.h"
#include "VertexData.h"


#include "Mesh.h"
#include "MaterialData.h"
#include "ModelData.h"
#include "DirectionalLight.h"
#include "Transformation.h"
#include <WorldTransform.h>
#include "Camera.h"
#include <CameraForGPU.h>
#include <Light/PointLight.h>

class Model {
public:

	//コンストラクタ
	Model();

	//初期化
	//Initializeも兼ねているよ
	//通常
	//static Model* Create(const std::string& directoryPath, const std::string& fileName);

	void CreateObj(const std::string& directoryPath, const std::string& fileName);

private:
#pragma region モデルの読み込み関係の関数
	//モデルデータの読み込み
	ModelData LoadObjectFile(const std::string& directoryPath, const std::string& fileName);

	//mtlファイルの読み込み
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);

#pragma endregion

public:
	//通常
	void Draw(WorldTransform& worldTransform, Camera& camera);

	//テクスチャ上書き用
	//void Draw(WorldTransform& worldTransform, Camera& camera,uint32_t textureHandle);


	//デストラクタ
	~Model();




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
	void SetBlendMode(int32_t blendmode) {
		blendModeNumber_ = blendmode;
	}

#pragma region Lightingの設定
	void SetLighting(bool enableLighting) {
		this->isEnableLighting_ = enableLighting;
	}
	//方向
	void SetDirection(Vector3 direction) {
		this->lightingDirection_ = direction;
	}
	//光沢
	void SetShiness(float shiness) {
		this->shiness_ = shiness;
	}

#pragma endregion



private:
	//頂点リソースを作る
	//頂点バッファビューを作成する
	//頂点リソースにデータを書き込む
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	std::vector<VertexData> vertices_{};






	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;



	//Lighting用
	//std::unique_ptr<CreateDirectionalLight> directionalLight_ = nullptr;
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;

	Vector4 lightColor_ = { 1.0f,1.0f,1.0f,1.0f };
	Vector3 direction_ = { 0.0f,-1.0f,0.0f };
	float intensity_ = 3.0f;




	//基本はtrueで
	bool isEnableLighting_ = true;
	
	//カメラ
	ComPtr<ID3D12Resource> cameraResource_ = nullptr;
	CameraForGPU* cameraForGPU_ = {};
	//光沢度
	float shiness_ = 80.0f;


	//PointLight
	ComPtr<ID3D12Resource> pointLightResource_ = nullptr;
	PointLight* pointLight_ = {};



	//方向
	Vector3 lightingDirection_ = { 0.0f,-1.0f,0.0f };



	uint32_t textureHandle_ = 0;



	//色関係のメンバ変数
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };




	//TextureManagerを参考にする
	static std::list<ModelData> modelInformationList_;


	//デフォルトはα加算
	int32_t blendModeNumber_;

};
#pragma once
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <array>
#include <memory>

#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "ImGuiManager/ImGuiManager.h"
#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Vector/Transform.h>
#include <Math/Vector/Material.h>
#include <Math/Matrix/Matrix/TransformationMatrix.h>
#include <Math/Vector/DirectionalLight.h>
#include "Polygon/Model/MaterialData/MaterialData.h"
#include "Polygon/Model/ModelData/ModelData.h"



#include "Math/Vector/Vector4.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"
#include <Math/Vector/VertexData.h>
#include "Math/Matrix/Matrix/WorldViewMatrix.h"

#include <Math/Vector/Calculation/VectorCalculation.h>
#include <externals/DirectXTex/d3dx12.h>




#include "Polygon/Model/Mesh/Mesh.h"
#include "Polygon/Model/Material/CreateMaterial.h"
#include "Polygon/Model/DirectionalLight/CreateDirectionalLight.h"
#include "Polygon/Model/Transformation/Transformation.h"
#include <Polygon/Model/Instancing/Instancing.h>



class Particle3D {
public:

	//コンストラクタ
	Particle3D();

	//初期化
	//Initializeも兼ねているよ
	//void CreateObject(const std::string& directoryPath,const std::string& fileName);


	static Particle3D* CreateRandomParticle(std::mt19937 randomEngine, const std::string& directoryPath,const std::string& fileName);


private:
#pragma region モデルの読み込み関係の関数
	//モデルデータの読み込み
	ModelData LoadObjectFile(const std::string& directoryPath, const std::string& fileName);

	//mtlファイルの読み込み
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);

#pragma endregion

public:
	//描画
	void Draw();


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
	void SetBlendMode(int32_t blendmode) {
		blendModeNumber_ = blendmode;
	}

	
	//アクセッサのまとめ

	//SRT
	//Scale
	void SetScale(Vector3 scale) {
		this->scale_ = scale;
	}
	const Vector3 GetScale() {
		return scale_;
	}
	//Rotate
	void SetRotate(Vector3 rotate) {
		this->rotate_ = rotate;
	}
	const Vector3 GetRotate() {
		return rotate_;
	}
	//Translate
	void SetTranslate(Vector3 translate) {
		this->translate_ = translate;
	}
	const Vector3 GetTranslate() {
		return translate_;
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
	static std::list<ModelData> modelInformationList_;


	//頂点データ
	std::unique_ptr<Mesh> mesh_ = nullptr;
	std::unique_ptr<Transformation> transformation_= nullptr ;
	std::vector<VertexData> vertices;
	//頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
		

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	std::vector<VertexData> vertices_{};



	//マテリアル用のリソースを作る
	std::unique_ptr<CreateMaterial> material_ = nullptr;
	//色関係のメンバ変数
	Vector4 color_;


	//Lighting用
	std::unique_ptr<CreateDirectionalLight> directionalLight_ = nullptr;
	//基本はtrueで
	bool isEnableLighting_ = true;
	//方向
	Vector3 lightingDirection_ = {0.0f,-1.0f,0.0f};


	//インスタンシングの数
	//static const int32_t instanceCount_ = 10;
	//インスタンシング
	std::unique_ptr<Instancing> instancing_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_ = {};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_ = {};

	ComPtr<ID3D12Resource>instancingResource_ = nullptr;

	static const int32_t MAX_INSTANCE_NUMBER_ = 10;
	//描画すべきインスタンス数
	uint32_t numInstance_ = 0;
	//パーティクル
	Particle particles_[MAX_INSTANCE_NUMBER_];
	ParticleForGPU* instancingData_ = nullptr;

	//SRT
	Vector3 scale_ = { 1.0f,1.0f,1.0f };
	Vector3 rotate_ = { 0.0f,0.0f,0.0f };
	Vector3 translate_ = { 0.0f,0.0f,0.0f };



	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//デフォルトはα加算
	int32_t blendModeNumber_=1 ;

};

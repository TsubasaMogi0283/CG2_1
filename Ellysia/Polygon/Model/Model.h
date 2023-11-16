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
#include "DirectionalLight/CreateDirectionalLight.h"
#include "Transformation/Transformation.h"


class Model {
public:

	//コンストラクタ
	Model();

	//初期化
	//Initializeも兼ねているよ
	//void CreateObject(const std::string& directoryPath,const std::string& fileName);


	static Model* Create(const std::string& directoryPath,const std::string& fileName);


private:
#pragma region モデルの読み込み関係の関数
	//モデルデータの読み込み
	ModelData LoadObjectFile(const std::string& directoryPath, const std::string& fileName);

	//mtlファイルの読み込み
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);

#pragma endregion

public:
	//描画
	void Draw(Transform transform);


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



private:

	

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);



	


private:
	//頂点リソースを作る
	//頂点バッファビューを作成する
	//頂点リソースにデータを書き込む

	//頂点データ
	std::unique_ptr<Mesh> mesh_ = nullptr;


	//Model用のTransformationMatrix用のリソースを作る。
	std::unique_ptr<Transformation> transformation_ = nullptr;

	//マテリアル用のリソースを作る
	std::unique_ptr<CreateMaterial> material_ = nullptr;

	//Lighting用
	std::unique_ptr<CreateDirectionalLight> directionalLight_ = nullptr;


	uint32_t textureHandle_ = 0;

	


	//色関係のメンバ変数
	Vector4 color_;


	

	//TextureManagerを参考にする
	static std::list<ModelData> modelInformationList_;
};
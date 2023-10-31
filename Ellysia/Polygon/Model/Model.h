#pragma once
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>

#include "Common/DirectX/DirectXSetup.h"
#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "ImGuiManager/ImGuiManager.h"
#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Vector/Transform.h>
#include <Math/Vector/Material.h>
#include <Math/Matrix/Matrix/TransformationMatrix.h>
#include <Math/Vector/DirectionalLight.h>
#include <TextureManager/MaterialData/MaterialData.h>
#include <TextureManager/ModelData/ModelData.h>



#include "Math/Vector/Vector4.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"
#include <Math/Vector/VertexData.h>
#include "Math/Matrix/Matrix/WorldViewMatrix.h"

#include <Math/Vector/Calculation/VectorCalculation.h>
#include <externals/DirectXTex/d3dx12.h>



class Model {
public:

	//コンストラクタ
	Model();

	//初期化
	//Initializeも兼ねているよ
	void CreateObject(const std::string& directoryPath,const std::string& fileName);

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

	//Resource作成の関数化
	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	//頂点バッファビューを作成する
	void GenerateVertexBufferView();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);




private:

	//DirectX内にある情報を取り入れる
	DirectXSetup* directXSetup_ = nullptr;


	//モデルの読み込み
	ModelData modelData_;

	//頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;


	//貯y店リソースにデータを書き込む
	VertexData* vertexData_;


	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;


	//Lighting用
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;

	uint32_t descriptorSizeSRV_ = 0u;

	ComPtr<ID3D12Resource> resource_ = nullptr;


	//構築するModelData
	ModelData modelData;


	//色関係のメンバ変数
	Vector4 color_;

	
};
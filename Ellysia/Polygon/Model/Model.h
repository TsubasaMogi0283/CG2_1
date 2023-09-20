#pragma once
#include "Math/Vector/ModelData.h"
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

class Model {
public:

	//コンストラクタ
	Model();

	//初期化
	void Initialize(DirectXSetup* directXSetup);

	//更新
	void Update();

	//描画
	void Draw(Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix);

	//解放
	void Release();

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	//頂点バッファビューを作成する
	void GenerateVertexBufferView();

	//Modelの読み込み
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& fileName);

	//デストラクタ
	~Model();




private:
	//DirectX内にある情報を取り入れる
	DirectXSetup* directXSetup_ = nullptr;

	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewModel_{};

	//モデルの読み込み
	ModelData modelData_;

	//頂点リソースを作る
	ID3D12Resource* vertexResource_ = nullptr;

	//頂点リソースにデータを書き込む
	VertexData* vertexData_ = nullptr;

	//TransformationMatrix用のリソースを作る
	ID3D12Resource* transformationMatrixResourceModel_ = nullptr;
	TransformationMatrix* transformationMatrixDataModel_ = nullptr;

	//マテリアル用のリソースを作る
	ID3D12Resource* materialResourceModel_ = nullptr;
	Material* materialData_ = nullptr;

	//Lighting用
	ID3D12Resource* directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;


	//Descriptorの設定
	uint32_t descriptorSizeSRV_=0u;
	uint32_t descriptorSizeRTV_=0u;
	uint32_t descriptorSizeDSV_=0u;

	
};


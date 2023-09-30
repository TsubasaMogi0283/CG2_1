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

	//モデルデータの読み込み
	ModelData LoadObjectFile(const std::string& directoryPath,const std::string& fileName);

	//更新
	void Update();

	//描画
	void Draw(Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix);

	//解放
	void Release();

	//デストラクタ
	~Model();


private:

	//Resource作成の関数化
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	//頂点バッファビューを作成する
	void GenerateVertexBufferView();
	



private:

	//DirectX内にある情報を取り入れる
	DirectXSetup* directXSetup_ = nullptr;


	//モデルの読み込み
	ModelData modelData_;

	//頂点リソースを作る
	ID3D12Resource* vertexResource_ = nullptr;
	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;


	//貯y店リソースにデータを書き込む
	VertexData* vertexData_;


};

#pragma once
#include "Math/Vector/ModelData.h"
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>

#include "Common/DirectX/DirectXSetup.h"
#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "externals/DirectXTex/DirectXTex.h"


class Model {
public:

	//コンストラクタ
	Model();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

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

	//モデルの読み込み
	ModelData modelData_;

	//頂点リソースを作る
	ID3D12Resource* vertexResource_ = nullptr;
};


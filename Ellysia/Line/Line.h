#pragma once
#include "DirectXSetup.h"
#include "ConvertLog.h"

#include "Vector4.h"
#include "Matrix4x4.h"
#include "Matrix4x4Calculation.h"
#include "VertexData.h"

#include <string>
#include "Material.h"
#include "TransformationMatrix.h"

struct LineMaterial {
	Vector4 color;
};

class Line {
public:
	//コンストラクタ
	Line();

	//初期化
	void Initialize();

	//描画
	void Draw(Vector3 start,Vector3 end);



	~Line();



private:

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};

	//Resource
	ComPtr<ID3D12Resource> vertexResouce_;

	//Resourceにデータを書き込む
	VertexData* vertexData_;

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	LineMaterial* materialData_ = nullptr;

	//TransformationMatrix用のResource
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	TransformationMatrix* wvpData_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;


};


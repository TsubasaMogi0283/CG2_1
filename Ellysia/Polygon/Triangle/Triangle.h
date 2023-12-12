#pragma once
#include "DirectXSetup.h"
#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "externals/DirectXTex/DirectXTex.h"

#include "Vector4.h"
#include "Math/Matrix/Matrix/Matrix4x4.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"
#include "Transform.h"
#include <VertexData.h>

#include <string>
#include <Material.h>
#include <Math/Matrix/Matrix/TransformationMatrix.h>

class Triangle {
public:
	//コンストラクタ
	Triangle();

	//初期化
	void Initialize();

	//描画
	void Draw(Vector4 left,Vector4 top,  Vector4 right,Transform transform,Vector4 color);



	~Triangle();



private:

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//Resource
	ComPtr<ID3D12Resource> vertexResouce_;

	//Resourceにデータを書き込む
	VertexData* vertexData_;
	
	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;
	
	//TransformationMatrix用のResource
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	TransformationMatrix* wvpData_=nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;


};

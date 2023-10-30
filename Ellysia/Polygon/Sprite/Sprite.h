#pragma once
#include "Common/DirectX/DirectXSetup.h"
#include "TextureManager/TextureManager.h"

#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "PipelineManager/PipelineManager.h"

#include "Math/Vector/Vector4.h"
#include "Math/Matrix/Matrix/Matrix4x4.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"
#include "Math/Vector/Transform.h"
#include <Math/Vector/VertexData.h>
#include "Math/Matrix/Matrix/WorldViewMatrix.h"
#include <Math/Vector/Material.h>

#include <string>
#include <format>
#include <Math/Vector/DirectionalLight.h>
#include <Math/Matrix/Matrix/TransformationMatrix.h>
#include "Math/Vector/SpritePosition.h"

class Sprite {
public:

	//コンストラクタ
	Sprite();

	
	void LoadTextureHandle(uint32_t textureHandle);


	//描画
	//左上、右上、左下、右下
	void DrawRect(Transform transform);


	//解放
	void Release();

	//デストラクタ
	~Sprite();


	
public:
#pragma region アクセッサ
	void SetAllPosition(SpritePosition spritePosition) {
		this->position_ = spritePosition;
	}








	//色
	void SetTransparency(float transparency) {
		this->color_.w = transparency;
	}
	void SetColor(Vector4 color) {
		this->color_ = color;
	}



#pragma endregion



private:
	//初期化
	void Initialize();
	
	//Resource作成の関数化
	//Buffer
	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
	//Vertex
	void CreateVertexBufferView();
	//Index
	void CreateIndexBufferView();


	//
	void AssertInformation();

private:
	DirectXSetup* directXSetup_ = nullptr;


#pragma region リソース

	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//Sprite用
	//三角形2枚
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Material* materialData_ = nullptr;
	


	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	ComPtr<ID3D12Resource> transformationMatrixResource_ = nullptr;
	TransformationMatrix* transformationMatrixData_ = nullptr;


	//描画
	VertexData* vertexData_ = nullptr;


	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	


	//index用
	ComPtr<ID3D12Resource> indexResource_ = nullptr;
	//IndexBufferViewを作成
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	//インデックスデータ
	uint32_t* indexData_ = nullptr;

	static const int MAX_TEXTURE_ = 20;


	//画像読み込み
	ComPtr<ID3D12Resource> textureResource_ = nullptr;
	ComPtr<ID3D12Resource> resource_ = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;


#pragma endregion



	Transform uvTransformSprite_ = {};

	int textureIndex_ = 0;
	


	//位置
	SpritePosition position_ = {};

	Vector4 leftBottom_ = {};
	Vector4 leftTop_ = {};
	Vector4 rightBottom_ = {};
	Vector4 rightTop_ = {};

	Vector4 color_ = {};


	uint32_t texturehandle_ = 0u;
	bool isLoadTexture_ = false;
};
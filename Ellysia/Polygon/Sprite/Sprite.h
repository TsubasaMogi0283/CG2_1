#pragma once
#include "Common/DirectX/DirectXSetup.h"
#include "ConvertFunction/ConvertLog/LogConvert.h"
#include "externals/DirectXTex/DirectXTex.h"

#include "Math/Vector/Vector4.h"
#include "Math/Matrix/Matrix/Matrix4x4.h"
#include "Math/Matrix/Calculation/Matrix4x4Calculation.h"
#include "Math/Vector/Transform.h"
#include <Math/Vector/VertexData.h>
#include "Math/Matrix/Matrix/WorldViewMatrix.h"
#include <Math/Vector/Material.h>

#include <string>
#include <Math/Vector/DirectionalLight.h>
#include <Math/Matrix/Matrix/TransformationMatrix.h>
#include "Math/Vector/SpritePosition.h"

class Sprite {
public:

	//コンストラクタ
	Sprite();

	

	//まとめた方がよさそう
	void LoadTexture(const std::string& filePath);
	

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
	void SetTransparency(float transparency) {
		this->color_.w = transparency;
	}


#pragma endregion



private:
	//初期化
	void Initialize();
	
	//Resource作成の関数化
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	//頂点バッファビューを作成する
	void GenerateVertexBufferView();



#pragma region テクスチャの読み込み
	//Textureデータを読む
	//1.TextureデータそのものをCPUで読み込む
	DirectX::ScratchImage LoadTextureData(const std::string& filePath);

	//2.DirectX12のTextureResourceを作る
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);

	//3.TextureResourceに1で読んだデータを転送する
	//void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages,ID3D12Device* device,ID3D12GraphicsCommandList* commandList);
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);


#pragma endregion


private:
	DirectXSetup* directXSetup_ = nullptr;

	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//Sprite用
	//三角形2枚
	ID3D12Resource* vertexResourceSprite_ = nullptr;

	//マテリアル用のリソースを作る
	ID3D12Resource* materialResourceSprite_ = nullptr;
	Material* materialDataSprite_ = nullptr;
	
	//Lighting用
	ID3D12Resource* directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightDataSprite_ = nullptr;

	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	ID3D12Resource* transformationMatrixResourceSprite_ = nullptr;
	TransformationMatrix* transformationMatrixDataSprite_ = nullptr;


	//描画
	VertexData* vertexDataSprite_ = nullptr;


	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_;




	//index用
	ID3D12Resource* indexResourceSprite_ = nullptr;

	//IndexBufferViewを作成
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite_{};

	//インデックスデータ
	uint32_t* indexData_ = nullptr;

	static const int MAX_TEXTURE_ = 20;
	bool isUsedTextureIndex;


	//画像読み込み
	ID3D12Resource* textureResource_ = nullptr;
	ID3D12Resource* resource_ = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;

	ID3D12Resource* intermediateResource_= nullptr ;


	Transform uvTransformSprite_ = {};

	int textureIndex_ = 0;
	


	//位置
	SpritePosition position_ = {};

	Vector4 leftBottom_ = {};
	Vector4 leftTop_ = {};
	Vector4 rightBottom_ = {};
	Vector4 rightTop_ = {};

	Vector4 color_ = {};


};

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
#include <Math/Vector/SphereStruct.h>
#include <Math/Vector/Material.h>
#include <Math/Matrix/Matrix/TransformationMatrix.h>
#include <Math/Vector/DirectionalLight.h>

class Sphere {
public:
	
	//コンストラクタ
	Sphere();

	//初期化
	void Initialize();

	//まとめた方がよさそう
	void LoadTexture(const std::string& filePath);
	
	//更新処理
	//最終手段。ここにImGui入れる
	void Update();


	//描画
	//左上、右上、左下、右下
	void Draw(SphereStruct sphereCondtion, Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix, Vector4 color);


	//解放
	void Release();

	//デストラクタ
	~Sphere();


private:

	
	//Resource作成の関数化
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	//頂点バッファビューを作成する
	void GenerateVertexBufferView();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

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

	//DirectX内にある情報を取り入れる
	DirectXSetup* directXSetup_ = nullptr;



	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_;

	//初期化
	//頂点データ
	ID3D12Resource* vertexResourceSphere_ = nullptr;
	VertexData* vertexDataSphere_ = nullptr;


	//constにする意味はあったのだろうか
	uint32_t descriptorSizeSRV_=0u;
	uint32_t descriptorSizeRTV_=0u;
	uint32_t descriptorSizeDSV_=0u;


	//マテリアル用のリソースを作る
	ID3D12Resource* materialResourceSphere_ = nullptr;
	Material* materialData_ = nullptr;

	//球を描画するとき用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	ID3D12Resource* transformationMatrixResourceSphere_ = nullptr;
	TransformationMatrix* transformationMatrixDataSphere_ = nullptr;

	//Lighting用
	ID3D12Resource* directionalLightResource_ = nullptr;
	DirectionalLight* directionalLightData_ = nullptr;



#pragma region テクスチャ

	//画像読み込み
	DirectX::ScratchImage mipImages_;
	DirectX::ScratchImage mipImages2_;

	ID3D12Resource* textureResource_ = nullptr;
	ID3D12Resource* resource_ = nullptr;

	ID3D12Resource* textureResource2_ = nullptr;

	

	ID3D12Resource* intermediateResource_ = nullptr;


	//1枚目用
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_;


	//2枚目用
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2_;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2_;



	//モンスターボールに変更するフラグ
	bool useMonsterBall_ = true;
	
#pragma endregion


	//分割数
	const int32_t SUBDIVISION_ = 16;

};
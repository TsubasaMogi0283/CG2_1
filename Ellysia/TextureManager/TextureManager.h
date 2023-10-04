#pragma once
#include <d3d12.h>
#include <string>

#include "externals/DirectXTex/DirectXTex.h"
#include <externals/DirectXTex/d3dx12.h>


#include "Common/DirectX/DirectXSetup.h"
#include <ConvertFunction/ConvertLog/LogConvert.h>

//テクスチャに関するクラス
class TextureManager {
public:
	//コンストラクタ
	TextureManager();

	//初期化
	void Initilalize(DirectXSetup* directXSetup);

	//統合させた関数
	int LoadTexture(const std::string& filePath);

	//解放
	void Release();


	//コンストラクタ
	~TextureManager();




private:


	//Resource作成の関数化
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

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
	//DirectX読み込み
	DirectXSetup* directXSetup_ = nullptr;

	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};


	static const int MAX_TEXTURE_ = 3;
	bool isUsedTextureIndex[MAX_TEXTURE_];

	ID3D12Resource* textureResource_[MAX_TEXTURE_] = {nullptr};
	ID3D12Resource* resource_ = nullptr;

	//画像読み込み
	DirectX::ScratchImage mipImages_;
	ID3D12Resource* intermediateResource_[MAX_TEXTURE_] = { nullptr };

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[MAX_TEXTURE_] = {} ;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[MAX_TEXTURE_] = {};

	uint32_t descriptorSizeSRV_ = 0u;
};
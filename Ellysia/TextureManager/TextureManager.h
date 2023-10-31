#pragma once
#include <d3d12.h>
#include <string>

#include "externals/DirectXTex/DirectXTex.h"
#include <externals/DirectXTex/d3dx12.h>


#include "Common/DirectX/DirectXSetup.h"
#include <ConvertFunction/ConvertLog/LogConvert.h>

//テクスチャに関するクラス
class TextureManager {
private:
	//コンストラクタ
	TextureManager();

	//コンストラクタ
	~TextureManager();

public:
	static TextureManager* GetInstance();


	//コピーコンストラクタ禁止
	TextureManager(const TextureManager& textureManager) = delete;

	//代入演算子を無効にする
	TextureManager& operator=(const TextureManager& textureManager) = delete;


public:
	
	//初期化
	static void Initilalize();

	//統合させた関数
	//インデックスを返すからマイナスはありえない。
	//uintにしたほうが良いよね
	static uint32_t LoadTexture(const std::string& filePath);

	static void TexCommand(uint32_t texHandle);

	//解放
	void Release();



private:


	//Resource作成の関数化
	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);



#pragma region テクスチャの読み込み
	//Textureデータを読む
	//1.TextureデータそのものをCPUで読み込む
	static DirectX::ScratchImage LoadTextureData(const std::string& filePath);

	//2.DirectX12のTextureResourceを作る
	static ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	//3.TextureResourceに1で読んだデータを転送する
	//void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages,ID3D12Device* device,ID3D12GraphicsCommandList* commandList);
	static void UploadTextureData(ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);


#pragma endregion
	

public:
		static const int TEXTURE_MAX_AMOUNT_ = 256;
private:

	

	ComPtr<ID3D12Resource> textureResource_[TEXTURE_MAX_AMOUNT_] = {nullptr};

	//画像読み込み
	
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[TEXTURE_MAX_AMOUNT_] = {};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[TEXTURE_MAX_AMOUNT_] = {};



};
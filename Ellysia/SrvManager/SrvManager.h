#pragma once
#include "DirectXSetup.h"


//SRVデスクリプタヒープを作る
//CPU・GPUハンドル取得


class SrvManager final{
public:
	//コンストラクタ
	SrvManager() = default;

	//デストラクタ
	~SrvManager() = default;

public:
	//インスタンス
	static SrvManager* GetInstance();

	//コピーコンストラクタ禁止
	SrvManager(const SrvManager& srvManager) = delete;

	//代入演算子を無効にする
	SrvManager& operator=(const SrvManager& srvManager) = delete;




public:
	//初期化
	void Initialize();

	uint32_t Allocate();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	ComPtr<ID3D12DescriptorHeap> GetSrvDescriptorHeap() {
		return  descriptorHeap_;
	}


	void PreDraw();

	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	void CreateSRVForStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

private:
	//最大SRV数(最大テクスチャ数)
	static const uint32_t MAX_SRV_COUNT_;

	//SRV用のデスクリプタサイズ
	uint32_t descriptorSize_;

	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;


	//画像読み込み
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU_ = {};
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU_ = {};

	//次に使用するSRVインデックス
	uint32_t useIndex_ = 0;
	


};


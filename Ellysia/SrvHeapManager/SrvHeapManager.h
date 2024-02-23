#pragma once
#include "DirectXSetup.h"


class SrvHeapManager final{
public:
	//コンストラクタ
	SrvHeapManager() = default;

	//デストラクタ
	~SrvHeapManager() = default;

public:
	//インスタンス
	static SrvHeapManager* GetInstance();

	//コピーコンストラクタ禁止
	SrvHeapManager(const SrvHeapManager& srvHeapManager) = delete;

	//代入演算子を無効にする
	SrvHeapManager& operator=(const SrvHeapManager& rvHeapManager) = delete;




public:
	//初期化
	void Initialize();

	uint32_t Allocate();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);


private:
	//最大SRV数(最大テクスチャ数)
	static const uint32_t MAX_SRV_COUNT_;

	//SRV用のデスクリプタサイズ
	uint32_t descriptorSize_;

	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;


	//次に使用するSRVインデックス
	uint32_t useIndex_ = 0;
	


};


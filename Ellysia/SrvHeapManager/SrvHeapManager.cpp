#include "SrvHeapManager.h"

const uint32_t SrvHeapManager::MAX_SRV_COUNT_ = 512;

/// <summary>
/// コンストラクタ
/// </summary>
SrvHeapManager::SrvHeapManager() {
	
}

/// <summary>
/// 初期化
/// </summary>
void SrvHeapManager::Initialize(){
	//デスクリプタヒープの生成
	SrvHeapManager::GetInstance()->descriptorHeap_ = DirectXSetup::GetInstance()->GenarateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, MAX_SRV_COUNT_, true);

	//デスクリプタ1個分のサイズを取得して記録
	SrvHeapManager::GetInstance()->descriptorSize_ = DirectXSetup::GetInstance()->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


}

uint32_t SrvHeapManager::Allocate(){

	//上限だったらasset
	assert(useIndex_ < MAX_SRV_COUNT_);

	//return する番号を一旦記録しておく
	int index = useIndex_;

	//次のために番号を1進める
	useIndex_++;
	
	//上で記録した番号をreturn
	
	return index;;
}



D3D12_CPU_DESCRIPTOR_HANDLE SrvHeapManager::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvHeapManager::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);
	return handleGPU;
}


/// <summary>
/// デストラクタ
/// </summary>
SrvHeapManager::~SrvHeapManager() {

}
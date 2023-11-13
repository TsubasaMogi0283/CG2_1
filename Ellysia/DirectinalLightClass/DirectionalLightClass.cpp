#include "DirectionalLightClass.h"

//Resource作成の関数化
ComPtr<ID3D12Resource> DirectionalLightClass::CreateBufferResource(size_t sizeInBytes) {
	//void返り値も忘れずに
	ComPtr<ID3D12Resource> resource = nullptr;
	
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD;

	//頂点リソースの設定
	
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc_.Width = sizeInBytes;
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc_.Height = 1;
	vertexResourceDesc_.DepthOrArraySize = 1;
	vertexResourceDesc_.MipLevels = 1;
	vertexResourceDesc_.SampleDesc.Count = 1;

	//バッファの場合はこれにする決まり
	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//hrは調査用
	HRESULT hr;
	hr = DirectXSetup::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}


void DirectionalLightClass::Initialize(){
	resource_ = CreateBufferResource(sizeof(DirectionalLight));
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	data_->color={ 1.0f,1.0f,1.0f,1.0f };
	data_->direction = { 0.0f,-1.0f,0.0f };
	data_->intensity = 3.0f;


}

void DirectionalLightClass::SetGraphicsCommand() {
	//lightingResourceの場所を設定
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, resource_->GetGPUVirtualAddress());
}

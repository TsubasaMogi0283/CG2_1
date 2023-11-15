#include "Mesh.h"
#include "Common/DirectX/DirectXSetup.h"

//Resource作成の関数化
ComPtr<ID3D12Resource> Mesh::CreateBufferResource(size_t sizeInBytes) {
	//void返り値も忘れずに
	ComPtr<ID3D12Resource> resource = nullptr;
	
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD;

	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
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

	//実際に頂点リソースを作る
	
	//次はここで問題
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


void Mesh::Initialize(const std::vector<VertexData>& vertices){
	vertices_ = vertices;

	vertexResource_ = CreateBufferResource(sizeof(VertexData) * vertices_.size());

	//読み込みのところでバッファインデックスを作った方がよさそう
	//vertexResourceがnullらしい
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation =vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * vertices_.size());
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, vertices_.data(), sizeof(VertexData) * vertices_.size());
	vertexResource_->Unmap(0, nullptr);
}

void Mesh::SetGraphicsCommand(){
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::DrawCall(){
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), 1, 0, 0);
}

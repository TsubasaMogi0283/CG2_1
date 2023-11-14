#include "MaterialData.h"
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>

//Resource作成の関数化
ComPtr<ID3D12Resource> MaterialDataClass::CreateBufferResource(size_t sizeInBytes) {
	//void返り値も忘れずに
	ComPtr<ID3D12Resource> resource = nullptr;
	
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD;

	//頂点リソースの設定
	//関数用
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

void MaterialDataClass::Initialize(){
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	resource_=CreateBufferResource(sizeof(Material)).Get();
}

void MaterialDataClass::DrawInformation(Vector4 color){

	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
	data_->color = color;
	data_->enableLighting=false;

	data_->uvTransform = MakeIdentity4x4();
}

void MaterialDataClass::SetGraphicCommand(){
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, resource_->GetGPUVirtualAddress());

}



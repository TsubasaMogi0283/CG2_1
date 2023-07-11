#include "Sprite.h"


//コンストラクタ
Sprite::Sprite(){

}


//Resource作成の関数化
ID3D12Resource* Sprite::CreateBufferResource(size_t sizeInBytes) {
	//void返り値も忘れずに
	ID3D12Resource* resource = nullptr;
	
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

	//実際に頂点リソースを作る
	//ID3D12Resource* vertexResource_ = nullptr;
	//hrは調査用
	HRESULT hr;
	hr = directXSetup_->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}


//頂点バッファビューを作成する
void Sprite::GenerateVertexBufferView() {
	
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResouce_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	
}


//初期化
void Sprite::Initialize(DirectXInitialization* directXSetup) {
	directXSetup_ = directXSetup;

	//
	//Triangleとほぼ同じ
	// 
	 
	//ここでBufferResourceを作る
	//Sprite用の頂点リソースを作る
	vertexResourceSprite_ = CreateBufferResource(sizeof(VertexData) * 6);


	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_=CreateBufferResource(sizeof(Vector4)* 3);

	

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = CreateBufferResource(sizeof(Matrix4x4));

	//頂点バッファビューを作成する
	GenerateVertexBufferView();

}


//描画
void Sprite::Draw(Vector4 left, Vector4 top, Vector4 right, Transform transform, Matrix4x4 viewMatrix, Matrix4x4 projectionMatrix, Vector4 color) {
	#pragma region Sprite用
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};

	//リソースの先頭のアドレス
	vertexBufferViewSprite.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	//私用するリソースのサイズは頂点６つ分のサイズ
	vertexBufferViewSprite.SizeInBytes = sizeof(VertexData) * 6;
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferViewSprite.StrideInBytes = sizeof(VertexData);


#pragma endregion
}

//デストラクタ
Sprite::~Sprite() {

}
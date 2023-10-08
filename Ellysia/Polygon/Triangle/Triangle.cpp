#include "Triangle.h"
#include "Math/Vector/Transform.h"
#include "Math/Matrix/Matrix/WorldViewMatrix.h"


//補助ライブラリ
#include "externals/DirectXTex/d3dx12.h"
//動的配列
#include <vector>

Triangle::Triangle() {
	
}



void Triangle::Initialize() {
	directXSetup_ = DirectXSetup::GetInstance();

	//ここでBufferResourceを作る
	//頂点を6に増やす
	vertexResouce_ = CreateBufferResource(sizeof(VertexData) * 6);
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_=CreateBufferResource(sizeof(Material));

	

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = CreateBufferResource(sizeof(TransformationMatrix));

	//頂点バッファビューを作成する
	GenerateVertexBufferView();



}


//Resource作成の関数化
ID3D12Resource* Triangle::CreateBufferResource(size_t sizeInBytes) {
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
	hr_ = directXSetup_->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr_));

	return resource;



}




//Textureデータを読む
////1.TextureデータそのものをCPUで読み込む
//DirectX::ScratchImage LoadTextureData(const std::string& filePath);
//
////2.DirectX12のTextureResourceを作る
//ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
//
////3.TextureResourceに1で読んだデータを転送する
//void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);


//Before
void Triangle::LoadTexture(const std::string& filePath) {
	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = LoadTextureData(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource_ = CreateTextureResource(directXSetup_->GetDevice(), metadata);
	intermediateResource_ = UploadTextureData(textureResource_, mipImages);
	

	//ShaderResourceView
	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//2Dテクスチャ
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	
	//SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU_ = directXSetup_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU_ = directXSetup_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU_.ptr += directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU_.ptr += directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの生成
	directXSetup_->GetDevice()->CreateShaderResourceView(textureResource_, &srvDesc, textureSrvHandleCPU_);

	

}

//After
//1.TextureデータそのものをCPUで読み込む
//2.DirectX12のTextureResourceを作る
//3.CPUで書き込む用にUploadHeapのResourceを作る(IntermediateResource)
//4.3に対してCPUでデータを書き込む
//5.CommandListに3を2に転送するコマンドを積む
//6.CommandQueueを使って実行する
//7.6の実行完了を待つ


#pragma region 上のLoadTextureにまとめた
//Textureを読み込むためのLoad関数
//1.TextureデータそのものをCPUで読み込む
DirectX::ScratchImage Triangle::LoadTextureData(const std::string& filePath) {
	
	//テクスチャファイルを読んでプログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));
	
	//ミップマップの作成
	//ミップマップ...元画像より小さなテクスチャ群
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//ミップマップ月のデータを返す
	return mipImages;
}

//2.DirectX12のTextureResourceを作る
ID3D12Resource* Triangle::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
	//1.metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	//Textureの幅
	resourceDesc.Width = UINT(metadata.width);
	//Textureの高さ
	resourceDesc.Height = UINT(metadata.height);
	//mipmapの数
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	//奥行き or 配列Textureの配列数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	//TextureのFormat
	resourceDesc.Format = metadata.format;
	//サンプリングカウント
	resourceDesc.SampleDesc.Count = 1;
	//Textureの次元数。普段使っているのは2次元
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	//2.利用するHeapの設定
	//利用するHeapの設定。非常に特殊な運用。02_04exで一般的なケース版がある
	D3D12_HEAP_PROPERTIES heapProperties{};
	//細かい設定を行う
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	//WriteBackポリシーでCPUアクセス可能
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//プロセッサの近くに配置
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	


	////WriteBackポリシーでCPUアクセス可能
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	////プロセッサの近くに配置
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	

	//3.Resourceを生成する
	
	HRESULT hr = directXSetup_->GetDevice()->CreateCommittedResource(
		&heapProperties,					//Heapの設定
		D3D12_HEAP_FLAG_NONE,				//Heapの特殊な設定
		&resourceDesc,						//Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,	//初回のResourceState。データの転送を受け入れられるようにする
		nullptr,							//Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource_));			//作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));

	return resource_;


}




//3.TextureResourceに1で読んだデータを転送する
//書き換え
[[nodiscard]]
ID3D12Resource* Triangle::UploadTextureData(
	ID3D12Resource* texture, 
	const DirectX::ScratchImage& mipImages) {

	std::vector<D3D12_SUBRESOURCE_DATA>subresource;
	DirectX::PrepareUpload(directXSetup_->GetDevice(), mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresource);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresource.size()));
	ID3D12Resource* intermediateResource = CreateBufferResource(intermediateSize);
	UpdateSubresources(directXSetup_->GetCommandList(), texture, intermediateResource, 0, 0, UINT(subresource.size()), subresource.data());
	
	//Textureへの転送後は利用出来るようD3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture ;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	directXSetup_->GetCommandList()->ResourceBarrier(1, &barrier);
	return intermediateResource;


}


#pragma endregion

//頂点バッファビューを作成する
void Triangle::GenerateVertexBufferView() {
	
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResouce_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	
}




//描画
void Triangle::Draw(Vector4 left, Vector4 top, Vector4 right, Transform transform, Matrix4x4 viewMatrix, Matrix4x4 projectionMatrix, Vector4 color) {

	//TextureCoordinate(テクスチャ座標系)
	//TexCoord,UV座標系とも呼ばれている

	//左上が(0,0)右下が(1,1)で画像全体を指定することが出来る
	//U(x)V(y)

	//書き込むためのアドレスを取得
	vertexResouce_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//左下
	vertexData_[0].position = { -0.5f,-0.5f,0.0f,1.0f };
	vertexData_[0].texCoord = { 0.0f,1.0f };
	//上
	vertexData_[1].position = {0.0f,0.5f,0.0f,1.0f};
	vertexData_[1].texCoord = { 0.5f,0.0f };
	//右下
	vertexData_[2].position = {0.5f,-0.5f,0.0f,1.0f} ;
	vertexData_[2].texCoord = { 1.0f,1.0f };
	//範囲外は危険だよ！！


	//左下2
	vertexData_[3].position = {-0.5f,-0.5f,0.5f,1.0f};
	vertexData_[3].texCoord = { 0.0f,1.0f };
	//上2
	vertexData_[4].position = {0.0f,0.0f,0.0f,1.0f};
	vertexData_[4].texCoord = { 0.5f,0.0f };
	//右下2
	vertexData_[5].position = {0.5f,-0.5f,-0.5f,1.0f} ;
	vertexData_[5].texCoord = { 1.0f,1.0f };


	//マテリアルにデータを書き込む
	

	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = color;
	materialData_->enableLighting = false;
	materialData_->uvTransform = MakeIdentity4x4();
	
	//materialResource_ = CreateBufferResource(directXSetup_->GetDevice(), sizeof(Vector4));
	
	
	//サイズに注意を払ってね！！！！！
	//どれだけのサイズが必要なのか考えよう

	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
	//遠視投影行列
	//Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.5f, float() / float(WINDOW_SIZE_HEIGHT), 0.1f, 100.0f);
	//Matrix4x4 worldMatrix = MakeAffineMatrix();
	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	//書き込む為のアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	
	
	
	//さっき作ったworldMatrixの情報をここに入れる
	wvpData_->WVP = worldViewProjectionMatrix;
	wvpData_->World = MakeIdentity4x4();
	



	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアルCBufferの場所を設定
	//ここでの[0]はregisterの0ではないよ。rootParameter配列の0番目
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//CBVを設定する
	//wvp用のCBufferの場所を指定
	//今回はRootParameter[1]に対してCBVの設定を行っている
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
	

	//描画(DrawCall)３頂点で１つのインスタンス。
	directXSetup_->GetCommandList()->DrawInstanced(6, 1, 0, 0);

}



void Triangle::Release() {
	vertexResouce_->Release();
	textureResource_->Release();
	materialResource_->Release();
	//Release忘れずに
	wvpResource_->Release();
	intermediateResource_->Release();
}

Triangle::~Triangle() {

}


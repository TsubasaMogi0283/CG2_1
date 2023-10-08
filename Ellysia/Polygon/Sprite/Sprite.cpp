#include "Sprite.h"



//補助ライブラリ
#include "externals/DirectXTex/d3dx12.h"
//動的配列
#include <vector>




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
	

	//vertexResourceがnullらしい
	//リソースの先頭のアドレスから使う
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 6;
	//１頂点あたりのサイズ
	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);
	
	//Indexを利用
	//リsp－スの先頭のアドレスから使う
	indexBufferViewSprite_.BufferLocation = indexResourceSprite_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferViewSprite_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferViewSprite_.Format = DXGI_FORMAT_R32_UINT;


}


//初期化
void Sprite::Initialize() {
	directXSetup_ = DirectXSetup::GetInstance();
	position_ = {};
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	//
	//Triangleとほぼ同じ
	// 
	
	//ここでBufferResourceを作る
	//Sprite用の頂点リソースを作る
	vertexResourceSprite_ = CreateBufferResource(sizeof(VertexData) * 6);

	//index用のリソースを作る
	indexResourceSprite_ = CreateBufferResource(sizeof(uint32_t) * 6);




	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResourceSprite_=CreateBufferResource(sizeof(Material));



	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	transformationMatrixResourceSprite_ = CreateBufferResource(sizeof(TransformationMatrix));
	



	uvTransformSprite_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f} 
	};


	//頂点バッファビューを作成する
	GenerateVertexBufferView();

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
void Sprite::LoadTexture(const std::string& filePath) {
	//ここで初期化いれて一つにしたい
	Initialize();

	//Textureを読んで転送する
	DirectX::ScratchImage mipImages = LoadTextureData(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource_ = CreateTextureResource(directXSetup_->GetDevice(), metadata);
	intermediateResource_= UploadTextureData(textureResource_, mipImages);
	

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
DirectX::ScratchImage Sprite::LoadTextureData(const std::string& filePath) {
	
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
ID3D12Resource* Sprite::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
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
ID3D12Resource* Sprite::UploadTextureData(
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


//描画
void Sprite::DrawRect(Transform transform) {
	
	//SetAllPosition
	leftBottom_ = {position_.leftBottom.x,position_.leftBottom.y,0.0f,1.0f};
	leftTop_ = {position_.leftTop.x,position_.leftTop.y,0.0f,1.0f};
	rightBottom_ = {position_.rightBottom.x,position_.rightBottom.y,0.0f,1.0f};
	rightTop_ = {position_.rightTop.x,position_.rightTop.y,0.0f,1.0f};


	//TextureCoordinate(テクスチャ座標系)
	//TexCoord,UV座標系とも呼ばれている

	//左上が(0,0)右下が(1,1)で画像全体を指定することが出来る
	//U(x)V(y)

	//今回は「Sprite」なので2枚必要
	//-------------
	//|\		  |
	//|  \		  |
	//|    \	  |
	//|      \    |
	//|         \ |
	//-------------

	//13
	//02



	//「結合した」のでvertexDataに書き込むものが減るよ。やったね
	//4つだけだよ

	//書き込むためのアドレスを取得
	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));
	//1枚目の三角形
	//左下
	vertexDataSprite_[0].position = {leftBottom_};
	vertexDataSprite_[0].texCoord = { 0.0f,1.0f };

	//左上
	vertexDataSprite_[1].position = {leftTop_};
	vertexDataSprite_[1].texCoord = { 0.0f,0.0f };
	
	//右下
	vertexDataSprite_[2].position = {rightBottom_} ;
	vertexDataSprite_[2].texCoord = { 1.0f,1.0f };


	//右上
	vertexDataSprite_[3].position = { rightTop_ };
	vertexDataSprite_[3].texCoord = { 1.0f,0.0f };



	//IndexResourceにデータを書き込む
	//インデックスデータにデータを書き込む
	indexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;



	//サイズに注意を払ってね！！！！！
	//どれだけのサイズが必要なのか考えよう

	transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));
	

	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
	//遠視投影行列
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(directXSetup_->GetClientWidth()), float(directXSetup_->GetClientHeight()), 0.0f, 100.0f);
	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));

	transformationMatrixDataSprite_->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixDataSprite_->World = MakeIdentity4x4();



	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite_));
	materialDataSprite_->color = color_;
	//ライティングしない
	materialDataSprite_->enableLighting = false;
	//materialDataSprite_->uvTransform = MakeIdentity4x4();
	

	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSprite_.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite_.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite_.translate));
	materialDataSprite_->uvTransform = uvTransformMatrix;


	//コマンドを積む

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite_);


	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite_->GetGPUVirtualAddress());
	//CBVを設定する
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);
	//Light
	//directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());


	////描画(DrawCall)３頂点で１つのインスタンス。
	//directXSetup_->GetCommandList()->DrawInstanced(6, 1, 0, 0);

	//今度はこっちでドローコールをするよ
	//描画(DrawCall)6個のインデックスを使用し1つのインスタンスを描画。
	directXSetup_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);


}

//解放
void Sprite::Release() {
	vertexResourceSprite_->Release();

	materialResourceSprite_->Release();
	
	transformationMatrixResourceSprite_->Release();
	
	indexResourceSprite_->Release();

	textureResource_->Release();
	resource_->Release();
	intermediateResource_->Release();
}

//デストラクタ
Sprite::~Sprite() {

}
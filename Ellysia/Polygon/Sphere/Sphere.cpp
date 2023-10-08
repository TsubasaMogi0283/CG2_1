#include "Sphere.h"

//補助ライブラリ
#include "externals/DirectXTex/d3dx12.h"
//動的配列
#include <vector>
#include <numbers>
#include <Math/Vector/Calculation/VectorCalculation.h>



//コンストラクタ
Sphere::Sphere() {

}

//Resource作成の関数化
ID3D12Resource* Sphere::CreateBufferResource(size_t sizeInBytes) {
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

D3D12_CPU_DESCRIPTOR_HANDLE Sphere::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE Sphere::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}



//頂点バッファビューを作成する
void Sphere::GenerateVertexBufferView() {
	

	//vertexResourceがnullらしい
	//リソースの先頭のアドレスから使う
	vertexBufferViewSphere_.BufferLocation = vertexResourceSphere_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferViewSphere_.SizeInBytes = sizeof(VertexData) * SUBDIVISION_ * SUBDIVISION_*6;
	//１頂点あたりのサイズ
	vertexBufferViewSphere_.StrideInBytes = sizeof(VertexData);
	

}


//初期化
void Sphere::Initialize() {
	this->directXSetup_ = DirectXSetup::GetInstance();

	//ここでBufferResourceを作る
	//頂点を6に増やす
	vertexResourceSphere_ = CreateBufferResource(sizeof(VertexData) * SUBDIVISION_ *SUBDIVISION_ * 6);
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResourceSphere_=CreateBufferResource(sizeof(Material));

	//Lighting
	directionalLightResource_ = CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color={ 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

	//Sphere用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	transformationMatrixResourceSphere_ = CreateBufferResource(sizeof(TransformationMatrix));

	//DescriptorSize
	descriptorSizeSRV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSizeRTV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeDSV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	//頂点バッファビューを作成する
	GenerateVertexBufferView();

	LoadTexture("Resources/uvChecker.png");
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


//統合
void Sphere::LoadTexture(const std::string& filePath) {
	//Textureを読んで転送する
	mipImages_ = LoadTextureData(filePath);
	const DirectX::TexMetadata& metadata = mipImages_.GetMetadata();
	textureResource_ = CreateTextureResource(directXSetup_->GetDevice(), metadata);
	intermediateResource_ = UploadTextureData(textureResource_, mipImages_);
	
	//2枚目のTextureを読んで転送する
	//いつか配列にする。2は何か嫌です。
	mipImages2_ = LoadTextureData("Resources/monsterBall.png");
	const DirectX::TexMetadata& metadata2 = mipImages2_.GetMetadata();
	textureResource2_ = CreateTextureResource(directXSetup_->GetDevice(), metadata2);
	UploadTextureData(textureResource2_, mipImages2_);


	//ShaderResourceView
	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//2Dテクスチャ
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	
	//2枚目のSRVを作る
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//2Dテクスチャ
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);
	


	//textureSrvHandleCPU_ = directXSetup_->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	//textureSrvHandleGPU_ = directXSetup_->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	
	//今のDEscriptorHeapには
	//0...ImGui
	//1...uvChecker
	//2...monsterBall
	//3...NULL
	//.
	//.
	//このような感じで入っている
	//後ろのindexに対応させる


	
	


	//SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU_ = GetCPUDescriptorHandle(directXSetup_->GetSrvDescriptorHeap(), descriptorSizeSRV_, 1);
	textureSrvHandleGPU_ = GetGPUDescriptorHandle(directXSetup_->GetSrvDescriptorHeap(), descriptorSizeSRV_, 1);

	
	textureSrvHandleCPU2_=GetCPUDescriptorHandle(directXSetup_->GetSrvDescriptorHeap(), descriptorSizeSRV_, 2);
	textureSrvHandleGPU2_=GetGPUDescriptorHandle(directXSetup_->GetSrvDescriptorHeap(), descriptorSizeSRV_, 2);

	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU_.ptr += directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU_.ptr += directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	textureSrvHandleCPU2_.ptr += directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU2_.ptr += directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの生成
	directXSetup_->GetDevice()->CreateShaderResourceView(textureResource_, &srvDesc, textureSrvHandleCPU_);
	directXSetup_->GetDevice()->CreateShaderResourceView(textureResource2_, &srvDesc2, textureSrvHandleCPU2_);

	

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
DirectX::ScratchImage Sphere::LoadTextureData(const std::string& filePath) {
	
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
ID3D12Resource* Sphere::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
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
ID3D12Resource* Sphere::UploadTextureData(
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

void Sphere::Update() {
	ImGui::Begin("Sphere");
	ImGui::Checkbox("useMonsterBall", &useMonsterBall_);
	ImGui::InputFloat3("color", &directionalLightData_->color.x);
	ImGui::SliderFloat3("color", &directionalLightData_->color.x, 0.0f, 1.0f);
	ImGui::InputFloat3("direction", &directionalLightData_->direction.x);
	ImGui::SliderFloat3("direction", &directionalLightData_->direction.x, -1.0f, 1.0f);
	ImGui::InputFloat("interity", &directionalLightData_->intensity);
	ImGui::SliderFloat("interity", &directionalLightData_->intensity, 0.0f, 1.0f);

	ImGui::End();
}


//描画
//左上、右上、左下、右下
void Sphere::Draw(SphereStruct sphereCondtion, Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix ,Vector4 color) {

	
	//書き込み用のアドレスを取得
	vertexResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere_));


	

	//経度分割1つ分の角度φd
	//円周率使いたいとき「std::numbers::pi」こうするらしい
	//<numbers>もインクルードしなければならない。めんどいね。
	//しかもdouble型。floatにしないとダメだね。腹立つね。
	const float LON_EVERY = float(std::numbers::pi) * 2.0f / float(SUBDIVISION_);

	//緯度分割1つ分の角度θd
	const float LAT_EVERY = float(std::numbers::pi)/float(SUBDIVISION_);


	//緯度の方向に分割
	for (int latIndex = 0; latIndex < SUBDIVISION_; ++latIndex) {
		//θ
		float lat = - float (std::numbers::pi) / 2.0f + LAT_EVERY * latIndex;
		//経度の方向に分割しながら線を描く
		for (int lonIndex = 0; lonIndex < SUBDIVISION_; ++lonIndex) {
			uint32_t start = (latIndex * SUBDIVISION_ + lonIndex) * 6;
			
			//texcoord専用のxy座標
			float u = float(lonIndex) / float(SUBDIVISION_);
			//下から0上が1になっていたので「1.0f-～」にして逆にする
			float v = 1.0f - float(latIndex) / float(SUBDIVISION_);

			//φ
			float lon = lonIndex * LON_EVERY;
			//頂点にデータを入力する。基準点a
			//書き込む数は6つだよ
			 
			//点間の距離
			float length = 1.0f / SUBDIVISION_;

#pragma region 三角形１枚目 
			//左上(点B)が原点
			//abc
			//資料通りだとここは点a(左下)
			vertexDataSphere_[start].position.x = sphereCondtion.radius*(cos(lat) * cos(lon));
			vertexDataSphere_[start].position.y = sphereCondtion.radius*(sin(lat));
			vertexDataSphere_[start].position.z = sphereCondtion.radius*(cos(lat) * sin(lon)) ;
			vertexDataSphere_[start].position.w = 1.0f;
			//分割分移動
			vertexDataSphere_[start].texCoord.x = u;
			vertexDataSphere_[start].texCoord.y = v + length;
			//法線情報を追加
			vertexDataSphere_[start].normal.x = vertexDataSphere_[start].position.x;
			vertexDataSphere_[start].normal.y = vertexDataSphere_[start].position.y;
			vertexDataSphere_[start].normal.z = vertexDataSphere_[start].position.z;

			//普通にstart+1していけばいいのかな
			//点b(左上)
			vertexDataSphere_[start + 1].position.x = sphereCondtion.radius*(cos(lat + LAT_EVERY)) * cos(lon);
			vertexDataSphere_[start + 1].position.y = sphereCondtion.radius*(sin(lat + LAT_EVERY)) ;
			vertexDataSphere_[start + 1].position.z = sphereCondtion.radius*(cos(lat + LAT_EVERY)) * sin(lon);
			vertexDataSphere_[start + 1].position.w = 1.0f;
			vertexDataSphere_[start + 1].texCoord.x = u;
			vertexDataSphere_[start + 1].texCoord.y = v;
			vertexDataSphere_[start + 1].normal.x = vertexDataSphere_[start + 1].position.x;
			vertexDataSphere_[start + 1].normal.y = vertexDataSphere_[start + 1].position.y;
			vertexDataSphere_[start + 1].normal.z = vertexDataSphere_[start + 1].position.z;


			//点c(右下)
			vertexDataSphere_[start + 2].position.x = sphereCondtion.radius*(cos(lat) * cos(lon + LON_EVERY)) ;
			vertexDataSphere_[start + 2].position.y = sphereCondtion.radius*(sin(lat));
			vertexDataSphere_[start + 2].position.z = sphereCondtion.radius*(cos(lat) * sin(lon + LON_EVERY));
			vertexDataSphere_[start + 2].position.w = 1.0f;
			vertexDataSphere_[start + 2].texCoord.x = u + length;
			vertexDataSphere_[start + 2].texCoord.y = v + length;
			vertexDataSphere_[start + 2].normal.x = vertexDataSphere_[start + 2].position.x;
			vertexDataSphere_[start + 2].normal.y = vertexDataSphere_[start + 2].position.y;
			vertexDataSphere_[start + 2].normal.z = vertexDataSphere_[start + 2].position.z;

#pragma endregion

#pragma region 三角形２枚目
			//bcd
			//点d(右上)
			vertexDataSphere_[start + 3].position.x = sphereCondtion.radius*(cos(lat + LAT_EVERY) * cos(lon + LON_EVERY));
			vertexDataSphere_[start + 3].position.y = sphereCondtion.radius*(sin(lat + LAT_EVERY));
			vertexDataSphere_[start + 3].position.z = sphereCondtion.radius*(cos(lat + LAT_EVERY) * sin(lon + LON_EVERY));
			vertexDataSphere_[start + 3].position.w = 1.0f;
			vertexDataSphere_[start + 3].texCoord.x = u + length;
			vertexDataSphere_[start + 3].texCoord.y = v;
			vertexDataSphere_[start + 3].normal.x = vertexDataSphere_[start + 3].position.x;
			vertexDataSphere_[start + 3].normal.y = vertexDataSphere_[start + 3].position.y;
			vertexDataSphere_[start + 3].normal.z = vertexDataSphere_[start + 3].position.z;


			//点c(右下)
			vertexDataSphere_[start + 4].position.x = sphereCondtion.radius*(cos(lat) * cos(lon + LON_EVERY));
			vertexDataSphere_[start + 4].position.y = sphereCondtion.radius*(sin(lat));
			vertexDataSphere_[start + 4].position.z = sphereCondtion.radius*(cos(lat) * sin(lon + LON_EVERY));
			vertexDataSphere_[start + 4].position.w = 1.0f;
			vertexDataSphere_[start + 4].texCoord.x = u + length;
			vertexDataSphere_[start + 4].texCoord.y = v + length;
			vertexDataSphere_[start + 4].normal.x = vertexDataSphere_[start + 4].position.x;
			vertexDataSphere_[start + 4].normal.y = vertexDataSphere_[start + 4].position.y;
			vertexDataSphere_[start + 4].normal.z = vertexDataSphere_[start + 4].position.z;



			//点b(左上)
			vertexDataSphere_[start + 5].position.x = sphereCondtion.radius*(cos(lat + LAT_EVERY) * cos(lon)) ;
			vertexDataSphere_[start + 5].position.y = sphereCondtion.radius*(sin(lat + LAT_EVERY));
			vertexDataSphere_[start + 5].position.z = sphereCondtion.radius*(cos(lat + LAT_EVERY) * sin(lon));
			vertexDataSphere_[start + 5].position.w = 1.0f;
			vertexDataSphere_[start + 5].texCoord.x = u;
			vertexDataSphere_[start + 5].texCoord.y = v;
			vertexDataSphere_[start + 5].normal.x = vertexDataSphere_[start + 5].position.x;
			vertexDataSphere_[start + 5].normal.y = vertexDataSphere_[start + 5].position.y;
			vertexDataSphere_[start + 5].normal.z = vertexDataSphere_[start + 5].position.z;

			
			

#pragma endregion
			

		}
	
	
	
	}

	

	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color;
	materialData_->enableLighting=true;

	materialData_->uvTransform = MakeIdentity4x4();

	//サイズに注意を払ってね！！！！！
	//どれだけのサイズが必要なのか考えよう

	transformationMatrixResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphere_));
	

	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transform.scale,transform.rotate,Add(transform.translate,sphereCondtion.center));
	//遠視投影行列
	Matrix4x4 viewMatrixSphere = MakeIdentity4x4();
	
	Matrix4x4 projectionMatrixSphere = MakeOrthographicMatrix(0.0f, 0.0f, float(directXSetup_->GetClientWidth()), float(directXSetup_->GetClientHeight()), 0.0f, 100.0f);
	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrixSphere = Multiply(worldMatrixSphere, Multiply(viewMatrix, projectionMatrix));


	transformationMatrixDataSphere_->WVP = worldViewProjectionMatrixSphere;
	transformationMatrixDataSphere_->World =MakeIdentity4x4();


	
	
	
	//コマンドを積む
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//CBVを設定する
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSphere_->GetGPUVirtualAddress());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere_);

	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSphere_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	//trueだったらtextureSrvHandleGPU2_
	directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2,useMonsterBall_?textureSrvHandleGPU2_:textureSrvHandleGPU_);
	//Light
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());


	//描画(DrawCall)３頂点で１つのインスタンス。
	directXSetup_->GetCommandList()->DrawInstanced(SUBDIVISION_*SUBDIVISION_*6, 1, 0, 0);

	


}


//解放
void Sphere::Release() {
	//初期化
	vertexResourceSphere_->Release();

	//マテリアル用のリソースを作る
	materialResourceSphere_->Release();
	

	//球を描画するとき用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	transformationMatrixResourceSphere_->Release();
	
	//画像読み込み
	textureResource_->Release();
	resource_->Release();

	intermediateResource_->Release();


}

//デストラクタ
Sphere::~Sphere() {

}

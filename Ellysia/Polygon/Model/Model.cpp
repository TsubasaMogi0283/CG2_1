#include "Model.h"
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>
#include <Math/Vector/Calculation/VectorCalculation.h>

//Model::Model() {
//
//}
//
////Resource作成の関数化
//ID3D12Resource* Model::CreateBufferResource(size_t sizeInBytes) {
//	//void返り値も忘れずに
//	ID3D12Resource* resource = nullptr;
//	
//	////VertexResourceを生成
//	//頂点リソース用のヒープを設定
//	
//	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD;
//
//	//頂点リソースの設定
//	
//	//バッファリソース。テクスチャの場合はまた別の設定をする
//	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	vertexResourceDesc_.Width = sizeInBytes;
//	//バッファの場合はこれらは1にする決まり
//	vertexResourceDesc_.Height = 1;
//	vertexResourceDesc_.DepthOrArraySize = 1;
//	vertexResourceDesc_.MipLevels = 1;
//	vertexResourceDesc_.SampleDesc.Count = 1;
//
//	//バッファの場合はこれにする決まり
//	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	//実際に頂点リソースを作る
//	//ID3D12Resource* vertexResource_ = nullptr;
//	//hrは調査用
//	HRESULT hr;
//	hr = directXSetup_->GetDevice()->CreateCommittedResource(
//		&uploadHeapProperties_,
//		D3D12_HEAP_FLAG_NONE,
//		&vertexResourceDesc_,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr, IID_PPV_ARGS(&resource));
//	assert(SUCCEEDED(hr));
//
//	return resource;
//}
//
////頂点バッファビューを作成する
////Sphereでもそうだけどまとめた方が良いかなと思った
//void Model::GenerateVertexBufferView() {
//	
//
//	//vertexResourceがnullらしい
//	//リソースの先頭のアドレスから使う
//	vertexBufferViewModel_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
//	//使用するリソースのサイズは頂点３つ分のサイズ
//	vertexBufferViewModel_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
//	//１頂点あたりのサイズ
//	vertexBufferViewModel_.StrideInBytes = sizeof(VertexData);
//	
//
//}
//

//void Model::Initialize(DirectXSetup* directXSetup) {
//	directXSetup_ = directXSetup;
//
//	//モデルの読み込み
//	//あとで外にだす
//	modelData_ = LoadObjFile("Resources/05_02","plane.obj");
//
//	//頂点リソースを作る
//	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
//
//	//Sphere用のTransformationMatrix用のリソースを作る。
//	//Matrix4x4 1つ分サイズを用意する
//	transformationMatrixResourceModel_ = CreateBufferResource(sizeof(TransformationMatrix));
//
//	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
//	materialResourceModel_=CreateBufferResource(sizeof(Material));
//
//
//	//Lighting
//	directionalLightResource_ = CreateBufferResource(sizeof(DirectionalLight));
//	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
//	directionalLightData_->color={ 1.0f,1.0f,1.0f,1.0f };
//	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
//	directionalLightData_->intensity = 1.0f;
//
//	//DescriptorSize
//	descriptorSizeSRV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//	descriptorSizeRTV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//	descriptorSizeDSV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
//
//
//
//	//頂点バッファビューを作成する
//	GenerateVertexBufferView();
//
//
//}
//
//void Model::Update() {
//	ImGui::Begin("Plane");
//	//ImGui::InputFloat3("rotate", &vertexData_->position);
//	ImGui::End();
//}
//
//void Model::Draw(Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix) {
//	//書き込むためのリソースを取得
//	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
//	
//	//頂点データをリソースにコピー
//	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
//
//
//
//
//	
//	//サイズに注意を払ってね！！！！！
//	//どれだけのサイズが必要なのか考えよう
//
//	transformationMatrixResourceModel_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataModel_));
//	
//
//	//新しく引数作った方が良いかも
//	Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
//	//遠視投影行列
//	Matrix4x4 viewMatrixSphere = MakeIdentity4x4();
//	
//	Matrix4x4 projectionMatrixSphere = MakeOrthographicMatrix(0.0f, 0.0f, float(directXSetup_->GetClientWidth()), float(directXSetup_->GetClientHeight()), 0.0f, 100.0f);
//	
//	//WVP行列を作成
//	Matrix4x4 worldViewProjectionMatrixSphere = Multiply(worldMatrixSphere, Multiply(viewMatrix, projectionMatrix));
//
//
//	transformationMatrixDataModel_->WVP = worldViewProjectionMatrixSphere;
//	transformationMatrixDataModel_->World =MakeIdentity4x4();
//
//
//	//マテリアルにデータを書き込む
//	//書き込むためのアドレスを取得
//	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
//	materialResourceModel_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
//	//色は白にしてモデル本来の色のままにしたい
//	//必要があればSetColorも作る。今のところは作らないけど
//	materialData_->color = {1.0f,1.0f,1.0f,1.0f};
//	materialData_->enableLighting=true;
//	
//	materialData_->uvTransform = MakeIdentity4x4();
//
//	
//	
//	//コマンドを積む
//	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
//	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//CBVを設定する
//	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceModel_->GetGPUVirtualAddress());
//
//	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
//	//directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewModel_);
//
//	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceModel_->GetGPUVirtualAddress());
//	//directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2, useMonsterBall_ ?textureSrvHandleGPU2_:textureSrvHandleGPU_);
//	//Light
//	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());
//
//
//
//	//ドローコール
//	directXSetup_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
//
//}
//
////解放
//void Model::Release() {
//	//DirectX内にある情報を取り入れる
//	//delete directXSetup_;
//
//	
//	//頂点リソースを作る
//	delete vertexResource_;
//
//	
//	//TransformationMatrix用のリソースを作る
//	delete transformationMatrixResourceModel_;
//	
//	//マテリアル用のリソースを作る
//	delete materialResourceModel_;
//	
//	//Lighting用
//	delete directionalLightResource_;
//
//}
//
//
//
//
//Model::~Model() {
//
//}
//



//補助ライブラリ
#include "externals/DirectXTex/d3dx12.h"
//動的配列
#include <vector>
#include <numbers>
#include <Math/Vector/Calculation/VectorCalculation.h>



//コンストラクタ
Model::Model() {

}

//Resource作成の関数化
ID3D12Resource* Model::CreateBufferResource(size_t sizeInBytes) {
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

D3D12_CPU_DESCRIPTOR_HANDLE Model::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE Model::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}



//頂点バッファビューを作成する
void Model::GenerateVertexBufferView() {
	

	//vertexResourceがnullらしい
	//リソースの先頭のアドレスから使う
	vertexBufferViewModel_.BufferLocation = vertexResourceSphere_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferViewModel_.SizeInBytes = UINT((sizeof(VertexData))*modelData_.vertices.size());
	//１頂点あたりのサイズ
	vertexBufferViewModel_.StrideInBytes = sizeof(VertexData);
	

}

//Modelの読み込み
ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& fileName) {
	//1.中で必要となる変数の宣言
	//構築するModelData
	ModelData modelData;
	//位置
	std::vector<Vector4> positions;
	//法線
	std::vector<Vector3> normals;
	//テクスチャ座標
	std::vector<Vector2> texcoords;
	//ファイルから読んだ1行を格納するもの
	std::string line;


	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + fileName);
	assert(file.is_open());
	 
	//3.実際にファイルを読み、ModelDataを構築していく

	//getline...streamから1行読んでstringに格納する
	//istringstream...文字列を分解しながら読むためのクラス、空白を区切りとして読む
	//objファイルの先頭にはその行の意味を示す識別子(identifier/id)が置かれているので、最初にこの識別子を読み込む

	//v...頂点位置
	//vt...頂点テクスチャ座標
	//vn...頂点法線
	//f...面

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		//先頭の識別子を読む
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "v") {
			Vector4 position;
			//ex).  v 「1.0000」 「1.0000」 「-0.0000」
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのINdexは「位置/uv/法線」で格納されているので、分解してindexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					// 「/」区切りでインデックスを読んでいく
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);

				}
				//要素へのIndexから実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);


			}
		}



	}


	//4.ModelDataを返す
	return modelData;
}



//初期化
void Model::Initialize(DirectXSetup* directXSetup) {
	this->directXSetup_ = directXSetup;

	
	modelData_ = LoadObjFile("Resources/05_02", "plane.obj");

	//ここでBufferResourceを作る
	vertexResourceSphere_ = CreateBufferResource(sizeof(VertexData)*modelData_.vertices.size());
	
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

	//LoadTexture("Resources/uvChecker.png");
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
void Model::LoadTexture(const std::string& filePath) {
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
DirectX::ScratchImage Model::LoadTextureData(const std::string& filePath) {
	
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
ID3D12Resource* Model::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata) {
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
ID3D12Resource* Model::UploadTextureData(
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

void Model::Update() {
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
void Model::Draw(Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix ) {

	
	//書き込み用のアドレスを取得
	vertexResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataModel_));

	//頂点データをリソースにコピー
	std::memcpy(vertexDataModel_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	
	/*D3D12_GPU_DESCRIPTOR_HANDLE texSrvHandleGPU = GetGPUDescriptorHandle(
		DirectXCommon::GetInstance()->GetSrvDescripterHeap(), descripterSize_.SRV, indexTex
	);


	texSrvHandleCPU.ptr += DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	texSrvHandleGPU.ptr += DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	*/
	

	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = {1.0f,1.0f,1.0f,1.0f};
	materialData_->enableLighting=false;

	materialData_->uvTransform = MakeIdentity4x4();

	//サイズに注意を払ってね！！！！！
	//どれだけのサイズが必要なのか考えよう

	transformationMatrixResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphere_));
	

	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
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
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewModel_);

	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSphere_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	//trueだったらtextureSrvHandleGPU2_
	directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2,useMonsterBall_?textureSrvHandleGPU2_:textureSrvHandleGPU_);
	//commands.List->SetGraphicsRootDescriptorTable(2, tex_.SrvHandleGPU);
	//Light
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());


	//利用する頂点数もModelDataの数を利用する
	directXSetup_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()),1,0,0);

	


}


//解放
void Model::Release() {
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
Model::~Model() {

}

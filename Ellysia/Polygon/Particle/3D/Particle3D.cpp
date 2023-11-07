#include "Particle3D.h"
#include <Camera/Camera.h>
#include <TextureManager/TextureManager.h>
#include <PipelineManager/PipelineManager.h>


Particle3D::Particle3D() {

}

D3D12_CPU_DESCRIPTOR_HANDLE Particle3D::GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE Particle3D::GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

//モデルデータの読み込み
ModelData Particle3D::LoadObjectFile(const std::string& directoryPath,const std::string& fileName) {
	//1.中で必要となる変数の宣言
	
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
			position.z *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.z *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			//面は三角形限定。その他は未対応
			VertexData triangle[3];
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
				//VertexData vertex = { position,texcoord,normal };
				//modelData.vertices.push_back(vertex);

				triangle[faceVertex] = { position,texcoord,normal };
				
				

			}
			//頂点を逆順で登録することで、回り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
			//左上
			modelData.vertices.push_back({ {1.0f,1.0f,0.0f,1.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f} });
			//右上
			modelData.vertices.push_back({ {-1.0f,1.0f,0.0f,1.0f},{1.0f,0.0f},{0.0f,0.0f,1.0f} });
			//左下
			modelData.vertices.push_back({ {1.0f,-1.0f,0.0f,1.0f},{0.0f,1.0f},{0.0f,0.0f,1.0f} });
			
			//左下
			modelData.vertices.push_back({ {1.0f,-1.0f,0.0f,1.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f} });
			//右上
			modelData.vertices.push_back({ {-1.0f,1.0f,0.0f,1.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f} });
			//右下
			modelData.vertices.push_back({ {-1.0f,-1.0f,0.0f,1.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f} });


		}
		else if (identifier == "mtllib") {
			//materialTemplateLibraryファイルの名前を取得する
			std::string materialFileName;
			s >> materialFileName;
			//基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFileName);
		}


	}

	////ここで問題
	//インデックス作った方がよさそう
	modelData.textureIndex = TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);

	
	

	//4.ModelDataを返す
	return modelData;
}

//mtlファイルを読む関数
MaterialData Particle3D::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName) {

	#pragma region 1.中で必要となる変数の宣言
	//構築するMaterialData
	MaterialData materialData;
	//ファイルから読んだ1行を格納するもの
	std::string line;
	
	#pragma endregion



	#pragma region 2.ファイルを開く
	std::ifstream file(directoryPath + "/" + fileName);
	//開かなかったら止める
	assert(file.is_open());

	#pragma endregion

	


	#pragma region  実際にファイルを読みMaterialDataを構築していく
	while (std::getline(file,line)){
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		//map_Kdにはtextureのファイル名が記載されているよ
		if (identifier == "map_Kd") {
			std::string textureFileName;
			s >> textureFileName;
			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFileName;

		}
		


	}

	#pragma endregion

	//4.MaterialDataを返す

	return materialData;



}


//Resource作成の関数化
ComPtr<ID3D12Resource> Particle3D::CreateBufferResource(size_t sizeInBytes) {
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

	//実際に頂点リソースを作る
	//ID3D12Resource* vertexResource_ = nullptr;
	
	//次はここで問題
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
void Particle3D::GenerateVertexBufferView() {
	

	//vertexResourceがnullらしい
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	

}






//初期化
void Particle3D::CreateObject(const std::string& directoryPath,const std::string& fileName) {
	
	this->directXSetup_ = DirectXSetup::GetInstance();

	//PSOの設定
	//全部のブレンドモードに対応させたい
	//switch文でやるのが良いかなと思った。切り替えが楽だから

	for (int i = 0; i < 3; i++) {
		randPosition_[i] =float( rnd_[i]() % 6 -2 );
		
	}

	for (int i = 0; i < PARTICLE_AMOUNT_; i++) {
		////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
		transform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1.0f,0.0f,0.0f} };
		materialResource_[i] = CreateBufferResource(sizeof(Material)).Get();

	}
	
	//モデルの読み込み
	//この２つ外に出した方がよさそう
	modelData_ = LoadObjectFile(directoryPath, fileName);
	//textureあった

	//頂点リソースを作る
	//モデルの頂点の数によって変わるよ
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size()).Get();
	
	//読み込みのところでバッファインデックスを作った方がよさそう
	GenerateVertexBufferView();
	
	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	for (int i = 0; i < PARTICLE_AMOUNT_; i++) {
		transformationMatrixResource_[i] = CreateBufferResource(sizeof(TransformationMatrix)).Get();
	
	}
	
	//Lighting
	for (int i = 0; i < PARTICLE_AMOUNT_; i++) {
		directionalLightResource_[i] = CreateBufferResource(sizeof(DirectionalLight)).Get();
		directionalLightResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
		directionalLightData_->color={ 1.0f,1.0f,1.0f,1.0f };
		directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
		directionalLightData_->intensity = 3.0f;

	}
	
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	
	

	//初期化の所でやってね、Update,Drawでやるのが好ましいけど凄く重くなった。
	//ブレンドモードの設定
	PipelineManager::GetInstance()->SetModelBlendMode(blendModeNumber_);
	PipelineManager::GetInstance()->GenerateModelPSO();
	
}

void Particle3D::Update() {
	ImGui::Begin("Plane");
	ImGui::SliderFloat3("Translate", &transform_[0].translate.x, -5.0f, 5.0f);
	ImGui::End();
}


//描画
void Particle3D::Draw() {
	//書き込むためのデータを書き込む
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//頂点データをリソースにコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	
	

	



	for (int i = 0; i < PARTICLE_AMOUNT_; i++) {
		transformationMatrixResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	
	}
	

	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	for (int i = 0; i < PARTICLE_AMOUNT_; i++) {
		materialResource_[i]->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	
	}
	materialData_->color = color_;
	materialData_->enableLighting = true;

	materialData_->uvTransform = MakeIdentity4x4();

	
	
	
	//Vector3 randTransform = { transform_.translate.x+randPosition_[0],transform_.translate.y+randPosition_[1],transform_.translate.z+randPosition_[2]};

	//新しく引数作った方が良いかも
	for (int i = 0; i < PARTICLE_AMOUNT_; i++) {
		Matrix4x4 worldMatrix = MakeAffineMatrix(transform_[i].scale, transform_[i].rotate, Add(transform_[i].translate, {0.0f,0.0f,0.0f}));

		//WVP行列を作成
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(Camera::GetInstance()->GetViewMatrix(), Camera::GetInstance()->GetProjectionMatrix_()));


		transformationMatrixData_->WVP = worldViewProjectionMatrix;
		transformationMatrixData_->World =MakeIdentity4x4();



	
	}
	


	//コマンドを積む

	directXSetup_->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetModelRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetModelGraphicsPipelineState().Get());


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	for (int i = 0; i < PARTICLE_AMOUNT_; i++) {
		//CBVを設定する
		directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_[i]->GetGPUVirtualAddress());

		directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_[i]->GetGPUVirtualAddress());
		//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	
		if (modelData.textureIndex  != 0) {
			TextureManager::TexCommand(modelData.textureIndex );
		}
	
		//Light
		directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_[i]->GetGPUVirtualAddress());

	}

	

	directXSetup_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), PARTICLE_AMOUNT_, 0, 0);


}




//デストラクタ
Particle3D::~Particle3D() {

}
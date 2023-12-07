#include "Polygon/Particle/3D/Particle3D.h" 
#include <Camera/Camera.h>
#include <TextureManager/TextureManager.h>
#include <PipelineManager/PipelineManager.h>
#include "Common/DirectX/DirectXSetup.h"
#include "Camera/Camera.h"


#include "Polygon/Particle/3D/Particle.h"
static uint32_t modelIndex;
static uint32_t descriptorSizeSRV_ = 0u;


Particle3D::Particle3D() {

}



//モデルデータの読み込み
ModelData Particle3D::LoadObjectFile(const std::string& directoryPath,const std::string& fileName) {
	//1.中で必要となる変数の宣言
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

		}
		else if (identifier == "mtllib") {
			//materialTemplateLibraryファイルの名前を取得する
			std::string materialFileName;
			s >> materialFileName;
			//基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFileName);
		}


	}

	
	
	

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

//生成関数
Particle Instancing::MakeNewParticle(std::mt19937& randomEngine) {
	std::uniform_real_distribution<float> distribute(-1.0f, 1.0f);
	Particle particle;
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	particle.transform.translate = {distribute(randomEngine),distribute(randomEngine),distribute(randomEngine)};
	
	//速度
	particle.velocity = {distribute(randomEngine),distribute(randomEngine),distribute(randomEngine)};

	//Color
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	particle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
	

	//時間
	std::uniform_real_distribution<float> distTime(10.0f, 30.0f);
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;



	return particle;

}


//RandomParticle用

void Particle3D::CreateRandomParticle(std::mt19937 randomEngine, const std::string& directoryPath, const std::string& fileName) {
	//新たなModel型のインスタンスのメモリを確保
	//Particle3D* particle3D = new Particle3D();
	
	
	

	//初期化の所でやってね、Update,Drawでやるのが好ましいけど凄く重くなった。
	//ブレンドモードの設定
	//Addでやるべきとのこと
	PipelineManager::GetInstance()->GenerateParticle3DPSO();


	//すでにある場合はリストから取り出す
	for (ModelData modelData : modelInformationList_) {
		if (modelData.name == fileName) {
			

			
			////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
			material_= std::make_unique<CreateMaterial>();
			material_->Initialize();

			

			//テクスチャの読み込み
			textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);


			//頂点リソースを作る
			//Instancing
			//Transformationいらなかったっす
			//Meshも一緒に入っているよ
			/*instancing_ = std::make_unique<Instancing>();
			instancing_->Initialize(randomEngine,modelData.vertices);
			*/
			
			mesh_ = std::make_unique<Mesh>();
			mesh_->Initialize(vertices);

			//インスタンシング
			instancingResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * MAX_INSTANCE_NUMBER_);
			
			descriptorSizeSRV_ =  DirectXSetup::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			

			D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
			instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
			instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			instancingSrvDesc.Buffer.FirstElement = 0;
			instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			instancingSrvDesc.Buffer.NumElements = MAX_INSTANCE_NUMBER_;
			instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

			instancingSrvHandleCPU_ = DirectXSetup::GetInstance()->GetCPUDescriptorHandle(
				DirectXSetup::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);
			instancingSrvHandleGPU_ = DirectXSetup::GetGPUDescriptorHandle(
				DirectXSetup::GetInstance()->GetSrvDescriptorHeap(), descriptorSizeSRV_, 3);

			DirectXSetup::GetInstance()->GetDevice()->CreateShaderResourceView(
				instancingResource_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

			
			


			

			//for (std::list<Particle>::iterator particleIterator = particles_.begin();
			//	particleIterator != particles_.end(); ++particleIterator) {

			//	//(*particleIterator).particles_.push_back(MakeNewParticle(randomEngine));
			//	//(*particleIterator).particles_.push_back(MakeNewParticle(randomEngine));
			//	//(*particleIterator).particles_.push_back(MakeNewParticle(randomEngine));
			//	
			//	std::uniform_real_distribution<float> distribute(-1.0f, 1.0f);
			//	(*particleIterator).transform.scale = {1.0f,1.0f,1.0f};
			//	(*particleIterator).transform.rotate = {0.0f,0.0f,0.0f};
			//	(*particleIterator).transform.translate = { distribute(randomEngine),distribute(randomEngine),distribute(randomEngine) };

			//	

			//	//速度の設定
			//	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
			//	(*particleIterator).velocity = { distribute(randomEngine),distribute(randomEngine),distribute(randomEngine)};
			//	(*particleIterator).color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
			//	//particles_[index].color = Vector4(1.0f,1.0f,1.0f,1.0f);

			//	//時間
			//	//Color
			//	std::uniform_real_distribution<float> distTime(10.0f, 30.0f);
			//	(*particleIterator).lifeTime = distTime(randomEngine);
			//	(*particleIterator).currentTime = 0;

			//}

			//SRTの設定
			for (uint32_t index = 0; index < MAX_INSTANCE_NUMBER_; ++index) {
				std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
				particles_[index].transform.scale = {1.0f,1.0f,1.0f};
				particles_[index].transform.rotate = {0.0f,0.0f,0.0f};
				particles_[index].transform.translate = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };

				

				//速度の設定
				std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
				particles_[index].velocity = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine)};
				particles_[index].color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine),1.0f };
				//particles_[index].color = Vector4(1.0f,1.0f,1.0f,1.0f);

				//時間
				//Color
				std::uniform_real_distribution<float> distTime(10.0f, 30.0f);
				particles_[index].lifeTime = distTime(randomEngine);
				particles_[index].currentTime = 0;


				particles_[index] = MakeNewParticle(randomEngine);
			}
	



			//Lighting
			directionalLight_=std::make_unique<CreateDirectionalLight>();
			directionalLight_->Initialize();

			
			
			
			//初期は白色
			//モデル個別に色を変更できるようにこれは外に出しておく
			color_ = { 1.0f,1.0f,1.0f,1.0f };

			continue;


		}
	}

	//モデルの読み込み
	ModelData modelDataNew = LoadObjectFile(directoryPath, fileName);
	modelDataNew.name = fileName;
	modelInformationList_.push_back(modelDataNew);
	


	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	material_= std::make_unique<CreateMaterial>();
	material_->Initialize();


	//テクスチャの読み込み
	textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelDataNew.material.textureFilePath);


	//頂点リソースを作る
	//Instancing
	//Transformationいらなかったっす
	instancing_ = std::make_unique<Instancing>();
	instancing_->Initialize(randomEngine,modelDataNew.vertices);
	isBillBordMode_ = true;

	//Lighting
	directionalLight_=std::make_unique<CreateDirectionalLight>();
	directionalLight_->Initialize();

	
	


	//初期は白色
	//モデル個別に色を変更できるようにこれは外に出しておく
	color_ = { 1.0f,1.0f,1.0f,1.0f };


}



//描画
void Particle3D::Draw() {
	
	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用

	material_->SetInformation(color_,isEnableLighting_);

	//書き込むためのデータを書き込む
	//頂点データをリソースにコピー
	
	
	
	
	

	//コマンドを積む
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetParticle3DRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetParticle3DGraphicsPipelineState().Get());


	


	//mesh_->GraphicsCommand();
	
	////RootSignatureを設定。PSOに設定しているけど別途設定が必要
	//DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	////形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	//DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//CBVを設定する
	material_->GraphicsCommand();
	
	//Transformationいらなかったっす
	//その代わりにInstancing
	
	
	//DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);

	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	
	if (textureHandle_!= 0) {
		TextureManager::GraphicsCommand(textureHandle_ );

	}
	

	//Light
	directionalLight_->GraphicsCommand();
	
	//DrawCall
	instancing_->SetGraphicsCommand(isBillBordMode_);
}


//描画
void Particle3D::Draw(uint32_t textureHandle) {
	
	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用

	material_->SetInformation(color_,isEnableLighting_);

	//書き込むためのデータを書き込む
	//頂点データをリソースにコピー
	
	
	
	
	

	//コマンドを積む
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetParticle3DRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetParticle3DGraphicsPipelineState().Get());


	


	//mesh_->GraphicsCommand();
	
	////RootSignatureを設定。PSOに設定しているけど別途設定が必要
	//DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	////形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	//DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//CBVを設定する
	material_->GraphicsCommand();
	
	//Transformationいらなかったっす
	//その代わりにInstancing
	
	
	//DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);

	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	
	if (textureHandle_!= 0) {
		TextureManager::GraphicsCommand(textureHandle );

	}
	

	//Light
	directionalLight_->GraphicsCommand();
	
	//DrawCall
	instancing_->SetGraphicsCommand(isBillBordMode_);
}

//デストラクタ
Particle3D::~Particle3D() {

}



	
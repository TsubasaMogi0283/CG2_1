#include "Model.h"
#include <Camera.h>
#include <TextureManager.h>
#include <PipelineManager.h>
#include "DirectXSetup.h"

#include <imgui.h>
static uint32_t modelIndex;
std::list<ModelData> Model::modelInformationList_{};


Model::Model() {

}


//モデルデータの読み込み
ModelData Model::LoadObjectFile(const std::string& directoryPath, const std::string& fileName) {
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
MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName) {

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
	while (std::getline(file, line)) {
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



//生成
//Model* Model::Create(const std::string& directoryPath, const std::string& fileName) {
//	//新たなModel型のインスタンスのメモリを確保
//	Model* model = new Model();
//	PipelineManager::GetInstance()->SetModelBlendMode(1);
//	PipelineManager::GetInstance()->GenerateModelPSO();
//
//	//すでにある場合はリストから取り出す
//	for (ModelData modelData : modelInformationList_) {
//		if (modelData.name == fileName) {
//			////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
//			model->material_ = std::make_unique<CreateMaterial>();
//			model->material_->Initialize();
//
//			//テクスチャの読み込み
//			model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);
//
//			//頂点リソースを作る
//			model->mesh_ = std::make_unique<Mesh>();
//			model->mesh_->Initialize(modelData.vertices);
//
//
//			//Lighting
//			model->directionalLight_ = std::make_unique<CreateDirectionalLight>();
//			model->directionalLight_->Initialize();
//
//			//Pixel用のカメラ
//			model->cameraResource_= DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraForGPU)).Get();
//
//			//PointLight
//			model->pointLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(PointLight)).Get();
//			
//
//			//初期は白色
//			//モデル個別に色を変更できるようにこれは外に出しておく
//			model->color_ = { 1.0f,1.0f,1.0f,1.0f };
//
//			return model;
//		}
//	}
//
//	//モデルの読み込み
//	ModelData modelDataNew = model->LoadObjectFile(directoryPath, fileName);
//	modelDataNew.name = fileName;
//	modelInformationList_.push_back(modelDataNew);
//
//
//
//	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
//	model->material_ = std::make_unique<CreateMaterial>();
//	model->material_->Initialize();
//
//
//
//	//テクスチャの読み込み
//	model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelDataNew.material.textureFilePath);
//
//
//	//頂点リソースを作る
//	model->mesh_ = std::make_unique<Mesh>();
//	model->mesh_->Initialize(modelDataNew.vertices);
//
//
//
//
//
//	//Sprite用のTransformationMatrix用のリソースを作る。
//	//Matrix4x4 1つ分サイズを用意する
//	//model->transformation_=std::make_unique<Transformation>();
//	//model->transformation_->Initialize();
//
//	//Lighting
//	model->directionalLight_ = std::make_unique<CreateDirectionalLight>();
//	model->directionalLight_->Initialize();
//
//	//Pixel用のカメラ
//	model->cameraResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraForGPU)).Get();
//
//	//PointLight
//	model->pointLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(PointLight)).Get();
//
//
//	//初期は白色
//	//モデル個別に色を変更できるようにこれは外に出しておく
//	model->color_ = { 1.0f,1.0f,1.0f,1.0f };
//
//
//
//	return model;
//
//}


void Model::CreateObj(const std::string& directoryPath, const std::string& fileName) {
	PipelineManager::GetInstance()->SetModelBlendMode(1);
	PipelineManager::GetInstance()->GenerateModelPSO();

	//すでにある場合はリストから取り出す
	//for (ModelData modelData : modelInformationList_) {
	//	if (modelData.name == fileName) {
	//		////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	//		material_ = std::make_unique<CreateMaterial>();
	//		material_->Initialize();

	//		//テクスチャの読み込み
	//		textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);

	//		//頂点リソースを作る
	//		mesh_ = std::make_unique<Mesh>();
	//		mesh_->Initialize(modelData.vertices);


	//		//Lighting
	//		directionalLight_ = std::make_unique<CreateDirectionalLight>();
	//		directionalLight_->Initialize();

	//		//Pixel用のカメラ
	//		cameraResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraForGPU)).Get();

	//		//PointLight
	//		pointLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(PointLight)).Get();


	//		//初期は白色
	//		//モデル個別に色を変更できるようにこれは外に出しておく
	//		color_ = { 1.0f,1.0f,1.0f,1.0f };

	//		return;

	//	}
	//}

	//モデルの読み込み
	ModelData modelDataNew = LoadObjectFile(directoryPath, fileName);
	modelDataNew.name = fileName;
	modelInformationList_.push_back(modelDataNew);



	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Material)).Get();


	//テクスチャの読み込み
	textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelDataNew.material.textureFilePath);

	//頂点リソースを作る
	//mesh_ = std::make_unique<Mesh>();
	//mesh_->Initialize(modelDataNew.vertices);


	vertices_ = modelDataNew.vertices;

	vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * vertices_.size());

	//読み込みのところでバッファインデックスを作った方がよさそう
	//vertexResourceがnullらしい
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * vertices_.size());
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);






	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, vertices_.data(), sizeof(VertexData) * vertices_.size());








	//Lighting
	directionalLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DirectionalLight)).Get();

	//Pixel用のカメラ
	cameraResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraForGPU)).Get();

	//PointLight
	pointLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(PointLight)).Get();


	//初期は白色
	//モデル個別に色を変更できるようにこれは外に出しておく
	color_ = { 1.0f,1.0f,1.0f,1.0f };


}


void Model::Update() {




	ImGui::Begin("PointLight");
	ImGui::SliderFloat("Intensity", &pointLightIntensity_, 0.0f, 1.0f);
	ImGui::SliderFloat3("Position", &pointLightPosition_.x, -3.0f, 3.0f);


	ImGui::End();
}

//描画
void Model::Draw(WorldTransform& worldTransform, Camera& camera) {



#pragma region マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用

	Material* materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color_;
	materialData_->enableLighting = true;
	materialData_->shininess = shiness_;
	materialData_->uvTransform = MakeIdentity4x4();
	materialResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region GPUに送る方のカメラ

	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
	
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.worldMatrix_.m[3][0];
	cameraWorldPosition.y = camera.worldMatrix_.m[3][1];
	cameraWorldPosition.z = camera.worldMatrix_.m[3][2];


	cameraForGPU_->worldPosition = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);

#pragma endregion


#pragma region DirectionalLightの書き込み

	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = lightColor_;
	directionalLightData_->direction = pointLightPosition_;
	directionalLightData_->intensity = 0.0f;
	directionalLightResource_->Unmap(0, nullptr);

#pragma endregion
	pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	pointLightData_->color = { 1.0f,1.0f,1.0f };
	pointLightData_->intensity = pointLightIntensity_;
	pointLightData_->position = pointLightPosition_;
	pointLightResource_->Unmap(0, nullptr);


	//コマンドを積む

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetModelRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetModelGraphicsPipelineState().Get());


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//CBVを設定する
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource_->GetGPUVirtualAddress());
	


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(textureHandle_);
	}


	//Light
	//3
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());


	//rootParameters[4]
	//カメラ
	//worldTransformと一緒に計算するもの
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());

	//PhongReflectionはこっち
	//rootParameters[5]
	//Pixel用
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//PointLight
	//rootParameters[6]
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, pointLightResource_->GetGPUVirtualAddress());


	//DrawCall
	//mesh_->DrawCall(1);
	
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), 1, 0, 0);
}



//デストラクタ
Model::~Model() {

}
#include "Model.h"
#include <Math/Matrix/Calculation/Matrix4x4Calculation.h>
#include <Math/Vector/Calculation/VectorCalculation.h>

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

//頂点バッファビューを作成する
//Sphereでもそうだけどまとめた方が良いかなと思った
void Model::GenerateVertexBufferView() {
	

	//vertexResourceがnullらしい
	//リソースの先頭のアドレスから使う
	vertexBufferViewModel_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferViewModel_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//１頂点あたりのサイズ
	vertexBufferViewModel_.StrideInBytes = sizeof(VertexData);
	

}

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

void Model::Initialize(DirectXSetup* directXSetup) {
	directXSetup_ = directXSetup;

	//モデルの読み込み
	//あとで外にだす
	modelData_ = LoadObjFile("Resources/05_02","plane.obj");

	//頂点リソースを作る
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());

	//Sphere用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	transformationMatrixResourceModel_ = CreateBufferResource(sizeof(TransformationMatrix));

	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResourceModel_=CreateBufferResource(sizeof(Material));


	//Lighting
	directionalLightResource_ = CreateBufferResource(sizeof(DirectionalLight));
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color={ 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

	//DescriptorSize
	descriptorSizeSRV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSizeRTV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSizeDSV_=directXSetup_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);



	//頂点バッファビューを作成する
	GenerateVertexBufferView();


}

void Model::Update() {
	ImGui::Begin("Plane");
	//ImGui::InputFloat3("rotate", &vertexData_->position);
	ImGui::End();
}

void Model::Draw(Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix) {
	//書き込むためのリソースを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	
	//頂点データをリソースにコピー
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());




	
	//サイズに注意を払ってね！！！！！
	//どれだけのサイズが必要なのか考えよう

	transformationMatrixResourceModel_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataModel_));
	

	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transform.scale,transform.rotate,transform.translate);
	//遠視投影行列
	Matrix4x4 viewMatrixSphere = MakeIdentity4x4();
	
	Matrix4x4 projectionMatrixSphere = MakeOrthographicMatrix(0.0f, 0.0f, float(directXSetup_->GetClientWidth()), float(directXSetup_->GetClientHeight()), 0.0f, 100.0f);
	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrixSphere = Multiply(worldMatrixSphere, Multiply(viewMatrix, projectionMatrix));


	transformationMatrixDataModel_->WVP = worldViewProjectionMatrixSphere;
	transformationMatrixDataModel_->World =MakeIdentity4x4();


	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResourceModel_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//色は白にしてモデル本来の色のままにしたい
	//必要があればSetColorも作る。今のところは作らないけど
	materialData_->color = {1.0f,1.0f,1.0f,1.0f};
	materialData_->enableLighting=true;

	materialData_->uvTransform = MakeIdentity4x4();

	
	
	//コマンドを積む
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//CBVを設定する
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceModel_->GetGPUVirtualAddress());

	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewModel_);

	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceModel_->GetGPUVirtualAddress());
	directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2, useMonsterBall_ ?textureSrvHandleGPU2_:textureSrvHandleGPU_);
	//Light
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());



	//ドローコール
	directXSetup_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);

}

//解放
void Model::Release() {

}




Model::~Model() {

}


#include "Modelmanager.h"
#include <cassert>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


static uint32_t modelhandle;

//コンストラクタ
ModelManager::ModelManager() {

}



ModelManager* ModelManager::GetInstance() {
	//関数内static変数として宣言する
	static ModelManager instance;

	return &instance;
}


//モデルデータの読み込み
ModelData ModelManager::LoadFile(const std::string& directoryPath, const std::string& fileName) {
	//1.中で必要となる変数の宣言
	ModelData modelData;


	//assimpでobjを読む
	//assimpを利用してしてobjファイルを読んでいく
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	//メッシュがないのは対応しない
	assert(scene->HasMeshes());


	//3.実際にファイルを読み、ModelDataを構築していく

	//getline...streamから1行読んでstringに格納する
	//istringstream...文字列を分解しながら読むためのクラス、空白を区切りとして読む
	//objファイルの先頭にはその行の意味を示す識別子(identifier/id)が置かれているので、最初にこの識別子を読み込む


	//Meshを解析
	//Meshは複数のFaceで構成され、そのFaceは複数の頂点で構成されている
	//さらにSceneには複数のMeshが存在しているというわけであるらしい
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		//Normalなので法線がない時は止める
		assert(mesh->HasNormals());
		//TextureCoordsなのでTexCoordが無い時は止める
		assert(mesh->HasTextureCoords(0));

		//faceを解析する
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces;++faceIndex ) {
			aiFace& face = mesh->mFaces[faceIndex];
			//三角形のみサポート
			assert(face.mNumIndices == 3);
			//ここからFaceの中身であるVertexの解析を行っていく
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texCoord = { texcoord.x,texcoord.y };
				//aiProcess_MakeLeftHandedはz*=-1で、
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);



			}


		}

	}


	//Materialを解析する
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	//ModelDataを返す
	return modelData;
}

uint32_t ModelManager::LoadObject(const std::string& directoryPath, const std::string& fileName) {

	//一度読み込んだものはその値を返す
	//新規は勿論読み込みをする
	for (uint32_t i = 0; i < MODEL_MAX_AMOUNT_; i++) {
		//同じモデルを探す
		if (ModelManager::GetInstance()->modelInfromtion_[i].directoryPath == directoryPath &&
			ModelManager::GetInstance()->modelInfromtion_[i].filePath == fileName) {
			return ModelManager::GetInstance()->modelInfromtion_[i].handle;
		}
	}

	modelhandle++;

	//モデルの読み込み
	ModelData newModelData = ModelManager::GetInstance()->LoadFile(directoryPath, fileName);
	//新規登録
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].data = newModelData;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].directoryPath = directoryPath;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].filePath = fileName;
	ModelManager::GetInstance()->modelInfromtion_[modelhandle].handle = modelhandle;




	//値を返す
	return modelhandle;
}

/// <summary>
/// デストラクタ
/// </summary>
ModelManager::~ModelManager() {

}
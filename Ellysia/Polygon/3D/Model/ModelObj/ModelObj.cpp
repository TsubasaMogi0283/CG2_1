#include "ModelObj.h"
#include <Camera.h>
#include <TextureManager.h>
#include <PipelineManager.h>
#include "DirectXSetup.h"


static uint32_t modelIndex;
std::list<ModelData> ModelObj::modelInformationList_{};


ModelObj::ModelObj() {

}



ModelObj* ModelObj::Create(const std::string& directoryPath, const std::string& fileName) {
	//新たなModel型のインスタンスのメモリを確保
	ModelObj* model = new ModelObj();
	PipelineManager::GetInstance()->SetModelBlendMode(1);
	PipelineManager::GetInstance()->GenerateModelPSO();	

	//すでにある場合はリストから取り出す
	for (ModelData modelData : modelInformationList_) {
		if (modelData.name == fileName) {
			////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
			model->material_= std::make_unique<CreateMaterial>();
			model->material_->Initialize();



			//テクスチャの読み込み
			model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);


			
			//頂点リソースを作る
			model->mesh_ = std::make_unique<Mesh>();
			model->mesh_->Initialize(modelData.vertices);



			
			



			//Sprite用のTransformationMatrix用のリソースを作る。
			//Matrix4x4 1つ分サイズを用意する
			//model->transformation_=std::make_unique<Transformation>();
			//model->transformation_->Initialize();

			//Lighting
			model->directionalLight_=std::make_unique<CreateDirectionalLight>();
			model->directionalLight_->Initialize();


			//初期は白色
			//モデル個別に色を変更できるようにこれは外に出しておく
			model->color_ = { 1.0f,1.0f,1.0f,1.0f };

			return model;
		}
	}

	//モデルの読み込み
	//ModelData modelDataNew = model->LoadObjectFile(directoryPath, fileName);
	//modelDataNew.name = fileName;
	//modelInformationList_.push_back(modelDataNew);
	




	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	model->material_= std::make_unique<CreateMaterial>();
	model->material_->Initialize();



	//テクスチャの読み込み
	//model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelDataNew.material.textureFilePath);


	//頂点リソースを作る
	model->mesh_ = std::make_unique<Mesh>();
	//model->mesh_->Initialize(modelDataNew.vertices);





	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	//model->transformation_=std::make_unique<Transformation>();
	//model->transformation_->Initialize();

	//Lighting
	model->directionalLight_=std::make_unique<CreateDirectionalLight>();
	model->directionalLight_->Initialize();


	//初期は白色
	//モデル個別に色を変更できるようにこれは外に出しておく
	model->color_ = { 1.0f,1.0f,1.0f,1.0f };

		

	return model;

}

//ブレンドあり
ModelObj* ModelObj::Create(const std::string& directoryPath, const std::string& fileName, int32_t blendModeNumber) {
	//新たなModel型のインスタンスのメモリを確保
	ModelObj* model = new ModelObj();

	//すでにある場合はリストから取り出す
	for (ModelData modelData : modelInformationList_) {
		if (modelData.name == fileName) {
			////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
			model->material_= std::make_unique<CreateMaterial>();
			model->material_->Initialize();



			//テクスチャの読み込み
			model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);


			//頂点リソースを作る
			model->mesh_ = std::make_unique<Mesh>();
			model->mesh_->Initialize(modelData.vertices);
			
			//Lighting
			model->directionalLight_=std::make_unique<CreateDirectionalLight>();
			model->directionalLight_->Initialize();


			//初期は白色
			//モデル個別に色を変更できるようにこれは外に出しておく
			model->color_ = { 1.0f,1.0f,1.0f,1.0f };
			//初期化の所でやってね、Update,Drawでやるのが好ましいけど凄く重くなった。
			//ブレンドモードの設定
			PipelineManager::GetInstance()->SetModelBlendMode(blendModeNumber);
			PipelineManager::GetInstance()->GenerateModelPSO();

			return model;
		}
	}

	//モデルの読み込み
	//ModelData modelDataNew = model->LoadObjectFile(directoryPath, fileName);
	//modelDataNew.name = fileName;
	//modelInformationList_.push_back(modelDataNew);
	




	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	model->material_= std::make_unique<CreateMaterial>();
	model->material_->Initialize();



	//テクスチャの読み込み
	//model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelDataNew.material.textureFilePath);


	//頂点リソースを作る
	model->mesh_ = std::make_unique<Mesh>();
	//model->mesh_->Initialize(modelDataNew.vertices);



	//Lighting
	model->directionalLight_=std::make_unique<CreateDirectionalLight>();
	model->directionalLight_->Initialize();


	//初期は白色
	//モデル個別に色を変更できるようにこれは外に出しておく
	model->color_ = { 1.0f,1.0f,1.0f,1.0f };

	PipelineManager::GetInstance()->SetModelBlendMode(blendModeNumber);
	PipelineManager::GetInstance()->GenerateModelPSO();

	return model;
}



//描画
void ModelObj::Draw(WorldTransform& worldTransform) {
	////マテリアルにデータを書き込む
	////書き込むためのアドレスを取得
	////reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用

	material_->SetInformation(color_,isEnableLighting_);


	
	//コマンドを積む

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetModelRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetModelGraphicsPipelineState().Get());


	////RootSignatureを設定。PSOに設定しているけど別途設定が必要
	//DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &modelInformation_[modelIndex].vertexBufferView_);
	////形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	//DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh_->GraphicsCommand();


	//CBVを設定する
	material_->GraphicsCommand();

	//transformation_->SetGraphicCommand();
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.constBufferResource_->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	
	if (textureHandle_!= 0) {
		TextureManager::GraphicsCommand(textureHandle_ );

	}
	

	//Light
	directionalLight_->SetDirection(lightingDirection_);
	directionalLight_->GraphicsCommand();
	


	//DrawCall
	mesh_->DrawCall(1);
}


//デストラクタ
ModelObj::~ModelObj() {

}

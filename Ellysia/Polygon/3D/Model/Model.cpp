#include "Model.h"
#include <Camera.h>
#include <TextureManager.h>
#include <PipelineManager.h>
#include "DirectXSetup.h"


static uint32_t modelIndex;
std::list<ModelData> Model::modelInformationList_{};


Model::Model() {

}


Model* Model::Create(const std::string& directoryPath, const std::string& fileName) {
	//新たなModel型のインスタンスのメモリを確保
	Model* model = new Model();


	PipelineManager::GetInstance()->SetModelBlendMode(1);
	PipelineManager::GetInstance()->GenerateModelPSO();

	////すでにある場合はリストから取り出す
	//for (ModelData modelData : modelInformationList_) {
	//	if (modelData.name == fileName) {
	//		////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	//		model->material_= std::make_unique<CreateMaterial>();
	//		model->material_->Initialize();



	//		//テクスチャの読み込み
	//		model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);


	//		//頂点リソースを作る
	//		model->mesh_ = std::make_unique<Mesh>();
	//		model->mesh_->Initialize(modelData.vertices);





	//		//Sprite用のTransformationMatrix用のリソースを作る。
	//		//Matrix4x4 1つ分サイズを用意する
	//		model->transformation_=std::make_unique<Transformation>();
	//		model->transformation_->Initialize();

	//		//Lighting
	//		model->directionalLight_=std::make_unique<CreateDirectionalLight>();
	//		model->directionalLight_->Initialize();


	//		//初期は白色
	//		//モデル個別に色を変更できるようにこれは外に出しておく
	//		model->color_ = { 1.0f,1.0f,1.0f,1.0f };
	//		//初期化の所でやってね、Update,Drawでやるのが好ましいけど凄く重くなった。
	//		//ブレンドモードの設定
	//		PipelineManager::GetInstance()->SetModelBlendMode(1);
	//		PipelineManager::GetInstance()->GenerateModelPSO();	

	//		return model;
	//	}
	//}

	//モデルの読み込み
	ModelData modelDataNew = LoadObjectFile(directoryPath, fileName);
	modelDataNew.name = fileName;
	modelInformationList_.push_back(modelDataNew);
	

	////マテリアル用のリソースを作る。
	model->materialResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Material)).Get();

	//テクスチャの読み込み
	model->textureHandle_ = TextureManager::GetInstance()->LoadTexture(modelDataNew.material.textureFilePath);


	//頂点リソースを作る
	model->vertices_ = modelDataNew.vertices;
	model->vertexResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * model->vertices_.size());

	//読み込みのところでバッファインデックスを作った方がよさそう
	//リソースの先頭のアドレスから使う
	model->vertexBufferView_.BufferLocation = model->vertexResource_->GetGPUVirtualAddress();
	//使用するリソースは頂点のサイズ
	model->vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * model->vertices_.size());
	//１頂点あたりのサイズ
	model->vertexBufferView_.StrideInBytes = sizeof(VertexData);


	//Lighting
	model->directionalLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(DirectionalLight)).Get();


	//カメラ
	model->cameraResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(CameraForGPU)).Get();
	

	//PointLight
	model->pointLightResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(PointLight)).Get();
	


	//初期は白色
	//モデル個別に色を変更できるようにこれは外に出しておく
	model->materialColor_ = { 1.0f,1.0f,1.0f,1.0f };

		

	return model;

}




//描画
void Model::Draw(WorldTransform& worldTransform,Camera& camera) {
	

	
	//資料にはなかったけどUnMapはあった方がいいらしい
	//Unmapを行うことで、リソースの変更が完了し、GPUとの同期が取られる。
	//プログラムが安定するらしいとのこと

#pragma region 頂点バッファ
	//頂点バッファにデータを書き込む
	VertexData* vertexData = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));//書き込むためのアドレスを取得
	std::memcpy(vertexData, vertices_.data(), sizeof(VertexData) * vertices_.size());
	vertexResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region マテリアル
	////書き込むためのアドレスを取得
	////reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = materialColor_;
	materialData_->lightingKinds = isEnableLighting_;
	materialData_->shininess = shininess_;
	materialData_->uvTransform = MakeIdentity4x4();

	materialResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region DirectionalLight
	
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = lightColor_;
	directionalLightData_->direction = lightingDirection_;
	directionalLightData_->intensity = directionalLightIntensity_;
	directionalLightResource_->Unmap(0, nullptr);

#pragma endregion

#pragma region PixelShaderに送る方のカメラ
	cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPU_));
	Vector3 cameraWorldPosition = {};
	cameraWorldPosition.x = camera.worldMatrix_.m[3][0];
	cameraWorldPosition.y = camera.worldMatrix_.m[3][1];
	cameraWorldPosition.z = camera.worldMatrix_.m[3][2];

	cameraForGPU_->worldPosition = cameraWorldPosition;
	cameraResource_->Unmap(0, nullptr);
#pragma endregion

#pragma region 点光源
	//PointLight
	pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	pointLightData_->color = pointLightColor_;
	pointLightData_->position = pointLightPosition_;
	pointLightData_->intensity = pointLightIntensity_;
	pointLightData_->radius = pointLightRadius_;
	pointLightData_->decay = pointLightDecay_;

	pointLightResource_->Unmap(0, nullptr);

#pragma endregion

	//コマンドを積む

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetModelRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetModelGraphicsPipelineState().Get());
	
	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//Material
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());


	//資料見返してみたがhlsl(GPU)に計算を任せているわけだった
	//コマンド送ってGPUで計算
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, worldTransform.bufferResource_->GetGPUVirtualAddress());


	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	if (textureHandle_!= 0) {
		TextureManager::GraphicsCommand(textureHandle_ );
	}
	
	//DirectionalLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	//カメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera.bufferResource_->GetGPUVirtualAddress());
	
	//PixelShaderに送る方のカメラ
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(5, cameraResource_->GetGPUVirtualAddress());

	//PointLight
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(6, pointLightResource_->GetGPUVirtualAddress());


	//DrawCall
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(UINT(vertices_.size()), 1, 0, 0);
}




//デストラクタ
Model::~Model() {

}
#include "ModelObj.h"
#include <Camera.h>
#include <TextureManager.h>
#include <PipelineManager.h>
#include "DirectXSetup.h"


static uint32_t modelIndex;
std::list<ModelData> ModelObj::modelInformationList_{};


ModelObj::ModelObj() {

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

#include "Line.h"
#include <PipelineManager.h>

Line::Line(){

}

void Line::Initialize(){
	//ここでBufferResourceを作る
	//頂点を6に増やす
	vertexResouce_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * 2);
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(Material));



	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(TransformationMatrix));

	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResouce_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);


	PipelineManager::GetInstance()->SetModelBlendMode(1);
	PipelineManager::GetInstance()->GenerateModelPSO();


}

void Line::Draw(Vector3 start, Vector3 end){


	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetLineRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetLineGraphicsPipelineState().Get());


	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = MakeIdentity4x4();
	materialResource_->Unmap(0, nullptr);



	//LineMaterial
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	//線のトポロジー
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(2, 1, 0, 0);

}

Line::~Line(){

}

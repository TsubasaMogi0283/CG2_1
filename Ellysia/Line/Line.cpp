#include "Line.h"

Line::Line(){

}

void Line::Initialize(){
	//ここでBufferResourceを作る
	//頂点を6に増やす
	vertexResouce_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * 6);
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

}

void Line::Draw(Vector3 start, Vector3 end){




	//線のトポロジー
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//描画(DrawCall)３頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(2, 1, 0, 0);

}

Line::~Line(){

}

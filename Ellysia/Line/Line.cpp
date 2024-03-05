#include "Line.h"
#include <PipelineManager.h>

Line::Line(){

}

void Line::Initialize() {

	//ここでBufferResourceを作る
	//頂点を6に増やす
	vertexResouce_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(LineVertexData) * 2);
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(LineMaterial));

	color_ = { 1.0f,1.0f,1.0f,1.0f };

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(LineTransformMatrix));

	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResouce_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(LineVertexData) * 2;
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(LineVertexData);


	//パイプライン
	PipelineManager::GetInstance()->GenaratedLinePSO();
}





//void Line::Initialize(){
//	//ここでBufferResourceを作る
//	//頂点を6に増やす
//	vertexResouce_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(VertexData) * 2);
//	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
//	materialResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(LineMaterial));
//
//	//初期は白
//	color_ = { 1.0f,1.0f,1.0f,1.0f };
//
//	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
//	wvpResource_ = DirectXSetup::GetInstance()->CreateBufferResource(sizeof(LineTransformMatrix));
//
//	//頂点バッファビューを作成する
//	//リソースの先頭のアドレスから使う
//	vertexBufferView_.BufferLocation = vertexResouce_->GetGPUVirtualAddress();
//	//使用するリソースのサイズは頂点３つ分のサイズ
//	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 2;
//	//１頂点あたりのサイズ
//	vertexBufferView_.StrideInBytes = sizeof(VertexData);
//
//	//パイプライン
//	PipelineManager::GetInstance()->GenaratedLinePSO();
//
//
//}


//描画
void Line::Draw(Vector3 start, Vector3 end, Camera& camera) {

	//TextureCoordinate(テクスチャ座標系)
	//TexCoord,UV座標系とも呼ばれている

	//左上が(0,0)右下が(1,1)で画像全体を指定することが出来る
	//U(x)V(y)
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetLineRootSignature().Get());
	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetLineGraphicsPipelineState().Get());

	//書き込むためのアドレスを取得
	vertexResouce_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//左下
	vertexData_[0].position = { start.x,start.y,0.0f,1.0f };
	//上
	vertexData_[1].position = { end.x,end.y,0.0f,1.0f };

	//マテリアルにデータを書き込む


	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	LineMaterial* materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	
	materialData_->color = color_;
	
	materialResource_->Unmap(0, nullptr);

	//サイズに注意を払ってね！！！！！
	//どれだけのサイズが必要なのか考えよう
	Vector3 scale = { 1.0f,1.0f,1.0f };
	Vector3 rotate = { 0.0f,0.0f,0.0f };
	//新しく引数作った方が良いかも
	Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, start);
	//遠視投影行列
	//Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.5f, float() / float(WINDOW_SIZE_HEIGHT), 0.1f, 100.0f);
	//Matrix4x4 worldMatrix = MakeAffineMatrix();
	//遠視投影行列
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();

	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WindowsSetup::GetInstance()->GetClientWidth()), float(WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);


	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrixSprite, projectionMatrixSprite));

	//書き込む為のアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));



	//さっき作ったworldMatrixの情報をここに入れる
	wvpData_->WVP = MakeIdentity4x4();
	wvpData_->World = worldMatrix;




	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//マテリアルCBufferの場所を設定
	
	//CBVを設定する
	//wvp用のCBufferの場所を指定
	//今回はRootParameter[1]に対してCBVの設定を行っている
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, wvpResource_->GetGPUVirtualAddress());

	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, camera.bufferResource_->GetGPUVirtualAddress());

	//マテリアル
	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, materialResource_->GetGPUVirtualAddress());

	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である

	/*if (textureHandle_ != 0) {
		TextureManager::GraphicsCommand(textureHandle_);

	}*/
	//directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_);


	//描画(DrawCall)6頂点で１つのインスタンス。
	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(2, 1, 0, 0);

}


//void Line::Draw(Vector3 start, Vector3 end,Camera& camera){
//
//
//	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetLineRootSignature().Get());
//	DirectXSetup::GetInstance()->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetLineGraphicsPipelineState().Get());
//
//
//	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
//	materialData_->color = color_;
//	materialResource_->Unmap(0, nullptr);
//
//
//
//	//書き込むためのアドレスを取得
//	vertexResouce_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
//
//	//左下
//	vertexData_[0].position = { -0.5f,-0.5f,0.0f,1.0f };
//	vertexData_[0].texCoord = { 0.0f,1.0f };
//	//上
//	vertexData_[1].position = { 0.0f,0.5f,0.0f,1.0f };
//	vertexData_[1].texCoord = { 0.5f,0.0f };
//
//	//LineはWorldTransformを使わない感じでやる
//	//ただカメラは使うよ
//
//	//サイズに注意を払ってね！！！！！
//	//どれだけのサイズが必要なのか考えよう
//
//	Vector3 scale = { 1.0f,1.0f,1.0f };
//	Vector3 rotate = { 0.0f,0.0f,0.0f };
//
//	//アフィンへ
//	Matrix4x4 startWorldMatrix = MakeAffineMatrix(scale, rotate, start);
//	Matrix4x4 endWorldMatrix = MakeAffineMatrix(scale, rotate, end);
//
//
//	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
//	//World
//	wvpData_->startWorld = startWorldMatrix;
//	//wvpData_->endWorld = endWorldMatrix;
//	
//	//WVP
//	wvpData_->startWVP = MakeIdentity4x4();
//	//wvpData_->endWVP = MakeIdentity4x4();
//
//	wvpResource_->Unmap(0, nullptr);
//
//
//	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
//	DirectXSetup::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
//
//	//線のトポロジー
//	DirectXSetup::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
//
//	//LineMaterial
//	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
//
//	//座標
//	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
//	//カメラ
//	//rootParameters[2],レジスタ番号1;
//	DirectXSetup::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera.bufferResource_->GetGPUVirtualAddress());
//
//
//	//描画(DrawCall)３頂点で１つのインスタンス。
//	DirectXSetup::GetInstance()->GetCommandList()->DrawInstanced(2, 1, 0, 0);
//
//}

Line::~Line(){

}
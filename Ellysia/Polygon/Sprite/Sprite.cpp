#include "Sprite.h"

#include "TextureManager/TextureManager.h"

//動的配列
#include <vector>




//コンストラクタ
Sprite::Sprite(){

}


//Resource作成の関数化
ComPtr<ID3D12Resource> Sprite::CreateBufferResource(size_t sizeInBytes) {
	//void返り値も忘れずに
	ComPtr<ID3D12Resource> resource = nullptr;
	
	////VertexResourceを生成
	//頂点リソース用のヒープを設定
	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
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

//Vertex
void Sprite::CreateVertexBufferView() {
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	//１頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

//Index
void Sprite::CreateIndexBufferView() {
	
	//リsp－スの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

}




//初期化
void Sprite::Initialize() {
	directXSetup_ = DirectXSetup::GetInstance();
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	
	

	
	//ここでBufferResourceを作る
	//Sprite用の頂点リソースを作る
	//以前三角形二枚にしてたけど結合して四角一枚で良くなったので4で良いよね
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * 6).Get();
	//index用のリソースを作る
	indexResource_ = CreateBufferResource(sizeof(uint32_t) * 6).Get();
	////マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_=CreateBufferResource(sizeof(Material));
	//Sprite用のTransformationMatrix用のリソースを作る。
	//Matrix4x4 1つ分サイズを用意する
	transformationMatrixResource_ = CreateBufferResource(sizeof(TransformationMatrix)).Get();
	

	//頂点バッファビューを作成する
	CreateVertexBufferView();

	//Indexを利用
	CreateIndexBufferView();


	uvTransformSprite_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f} 
	};


	
}

void Sprite::LoadTextureHandle(uint32_t textureHandle) {
	this->textureHandle_ = textureHandle;

	//テクスチャの情報を取得
	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);
	size_ = { float(resourceDesc_.Width),float(resourceDesc_.Height) };

	Initialize();

}
//描画
void Sprite::DrawRect() {
	
	//参考
	//assert(device_ != nullptr);


	//非表示にするかどうか
	if (isInvisible_ == true) {
		return;
	}

	

	//TextureCoordinate(テクスチャ座標系)
	//TexCoord,UV座標系とも呼ばれている

	//左上が(0,0)右下が(1,1)で画像全体を指定することが出来る
	//U(x)V(y)

	//今回は「Sprite」なので2枚必要
	//-------------
	//|\		  |
	//|  \		  |
	//|    \	  |
	//|      \    |
	//|         \ |
	//-------------

	//13
	//02



	//「結合した」のでvertexDataに書き込むものが減るよ。やったね
	//4つだけだよ

	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	float left = (0.0f-anchorPoint_.x) * size_.x;
	float right = (1.0f-anchorPoint_.x) * size_.x;
	float top = (0.0f-anchorPoint_.y) * size_.y;
	float bottom = (1.0f-anchorPoint_.y) * size_.y;

	//uv
	float texLeft = textureLeftTop_.x / resourceDesc_.Width;
	float texRight = (textureLeftTop_.x+textureSize_.x) / resourceDesc_.Width;
	float texTop= textureLeftTop_.y / resourceDesc_.Height;
	float texBottom= (textureLeftTop_.y +textureSize_.y)/ resourceDesc_.Height;



	//左右反転
	if (isFlipX_ == true) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_ == true) {
		top = -top;
		bottom = -bottom;
	}


	//1枚目の三角形
	//左下
	vertexData_[LEFT_BOTTOM].position = {left,bottom,0.0f,1.0f};
	vertexData_[LEFT_BOTTOM].texCoord = { texLeft,texBottom };

	//左上
	vertexData_[LEFT_TOP].position = {left,top,0.0f,1.0f};
	vertexData_[LEFT_TOP].texCoord = { texLeft,texTop };
	
	//右下
	vertexData_[RIGHT_BOTTOM].position = {right,bottom,0.0f,1.0f} ;
	vertexData_[RIGHT_BOTTOM].texCoord = { texRight,texBottom };


	//右上
	vertexData_[RIGHT_TOP].position = { right,top,0.0f,1.0f };
	vertexData_[RIGHT_TOP].texCoord = { texRight,texTop };


	//IndexResourceにデータを書き込む
	//インデックスデータにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;



	//サイズに注意を払ってね！！！！！
	//どれだけのサイズが必要なのか考えよう

	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	

	//新しく引数作った方が良いかも
	//3x3x3
	Matrix4x4 worldMatrixSprite = MakeAffineMatrix({ scale_.x,scale_.y,1.0f }, { 0.0f,0.0f,rotate_ }, {position_.x,position_.y,0.0f});
	//遠視投影行列
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(WindowsSetup::GetInstance()->GetClientWidth()), float(WindowsSetup::GetInstance()->GetClientHeight()), 0.0f, 100.0f);
	
	//WVP行列を作成
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));

	transformationMatrixData_->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixData_->World = MakeIdentity4x4();



	//マテリアルにデータを書き込む
	//書き込むためのアドレスを取得
	//reinterpret_cast...char* から int* へ、One_class* から Unrelated_class* へなどの変換に使用
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = color_;
	//ライティングしない
	materialData_->enableLighting = false;
	//materialDataSprite_->uvTransform = MakeIdentity4x4();
	

	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSprite_.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite_.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite_.translate));
	materialData_->uvTransform = uvTransformMatrix;


	//コマンドを積む
	//パイプラインはここに引っ越したい

	//参考
	directXSetup_->GetCommandList()->SetGraphicsRootSignature(PipelineManager::GetInstance()->GetSpriteRootSignature().Get());
	directXSetup_->GetCommandList()->SetPipelineState(PipelineManager::GetInstance()->GetSpriteGraphicsPipelineState().Get());


	//RootSignatureを設定。PSOに設定しているけど別途設定が必要
	directXSetup_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//IBVを設定
	directXSetup_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えよう
	directXSetup_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//CBVを設定する
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	directXSetup_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
	
	//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である
	//directXSetup_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetTextureIndex());
	
	
	if (textureHandle_ != 0) {
		TextureManager::TexCommand(textureHandle_);

	}
	
	//今度はこっちでドローコールをするよ
	//描画(DrawCall)6個のインデックスを使用し1つのインスタンスを描画。
	directXSetup_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);


}



//デストラクタ
Sprite::~Sprite() {

}
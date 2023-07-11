#pragma once
#include <Common/DirectX/DirectXInitialization.h>


#include <Math/Matrix/Matrix/Matrix4x4.h>
#include <Math/Vector/Transform.h>
#include <Math/Vector/VertexData.h>


class Sprite {
public:

	//コンストラクタ
	Sprite();

	//Resource作成の関数化
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	//頂点バッファビューを作成する
	void GenerateVertexBufferView();

	//初期化
	void Initialize(DirectXInitialization* directXSetup);

	//描画
	void Draw(Vector4 left,Vector4 top,  Vector4 right,Transform transform,Matrix4x4 viewMatrix,Matrix4x4 projectionMatrix ,Vector4 color);

	//デストラクタ
	~Sprite();


private:
	DirectXInitialization* directXSetup_ = nullptr;

	//関数用
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//Sprite用
	//三角形2枚
	ID3D12Resource* vertexResourceSprite_ = nullptr;

	//マテリアル用のリソースを作る
	ID3D12Resource* materialResource_ = nullptr;
	Vector4* materialData_ = nullptr;
	
	//TransformationMatrix用のResource
	ID3D12Resource* wvpResource_ = nullptr;
	Matrix4x4* wvpData_=nullptr;


	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	ID3D12Resource* vertexResouce_;



};

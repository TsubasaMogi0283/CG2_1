#pragma once
#include "Function.h"
#include "DirectXInitialization.h"

class Triangle {
public:
	Triangle();

	void Initialize(DirectXInitialization* directXSetup);

	void GenarateVertexResource();

	////VertexBufferViewを作成
	void GenerateVertexBufferView();

	//Material用のリソースを作る
	void GenerateMaterialResource();

	void Draw(Vector4 left,Vector4 top,  Vector4 right);


	//Resource作成の関数化
	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);


	void Release();
	
	~Triangle();



private:

	DirectXInitialization* directXSetup_ = nullptr;

	HRESULT hr_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

	//Resource
	ID3D12Resource* vertexResouce_;

	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};

	//Resourceにデータを書き込む
	Vector4* vertexData_ = nullptr;
	
	
	//マテリアル用のリソースを作る
	ID3D12Resource* materialResource = nullptr;

	
};
